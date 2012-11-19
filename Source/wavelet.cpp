//============================================================================
//
//  wavelet.cpp
//
//  Copyright (C) 2012  Sato Takaaki.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//============================================================================

#include <omp.h>
#include "wavelet.hpp"

namespace WaveletAnalyzer {

Wavelet::Wavelet() : m_PlanFFT(nullptr), m_PlanIFFT(nullptr),
        m_pTable(nullptr), m_pTemp0(nullptr), m_pTemp1(nullptr),
        m_pTemp2(nullptr), m_pTemp3(nullptr), m_TimeNum(0), m_FreqNum(0) {
    fftwf_init_threads();
}

Wavelet::~Wavelet() {
    fftwf_destroy_plan(m_PlanFFT);
    fftwf_destroy_plan(m_PlanIFFT);
    delete[] m_pTable;
    delete[] m_pTemp0;
    delete[] m_pTemp1;
    delete[] m_pTemp2;
    delete[] m_pTemp3;
}

bool Wavelet::Init(float tmin, float tmax, size_t tnum,
                   float fmin, float fmax, size_t fnum,
                   MotherWaveletFunc &MotherWavelet,
                   InterpolationFunc &Interpolation) {
    if(tnum <= 1 || fnum <= 1 || (tnum & (tnum - 1))) return false;
    fftwf_destroy_plan(m_PlanFFT);  m_PlanFFT  = nullptr;
    fftwf_destroy_plan(m_PlanIFFT); m_PlanIFFT = nullptr;
    delete[] m_pTable; m_pTable = nullptr;
    delete[] m_pTemp0; m_pTemp0 = nullptr;
    delete[] m_pTemp1; m_pTemp1 = nullptr;
    delete[] m_pTemp2; m_pTemp2 = nullptr;
    delete[] m_pTemp3; m_pTemp3 = nullptr;
    auto *table = new complex<float>[tnum * fnum];
    auto *temp0 = new float[tnum];
    auto *temp1 = new complex<float>[tnum];
    auto *temp2 = new complex<float>[tnum];
    auto *temp3 = new complex<float>[tnum];
    fftwf_plan fft, ifft;
    {
        auto in  = temp0;
        auto out = reinterpret_cast<fftwf_complex *>(temp1);
        fftwf_plan_with_nthreads(omp_get_max_threads());
        fft = fftwf_plan_dft_r2c_1d(fnum, in, out, FFTW_ESTIMATE);
    }
    {
        auto in  = reinterpret_cast<fftwf_complex *>(temp2);
        auto out = reinterpret_cast<fftwf_complex *>(temp3);
        fftwf_plan_with_nthreads(omp_get_max_threads());
        ifft = fftwf_plan_dft_1d(fnum, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    }
    m_PlanFFT  = fft;
    m_PlanIFFT = ifft;
    m_pTable   = table;
    m_pTemp0   = temp0;
    m_pTemp1   = temp1;
    m_pTemp2   = temp2;
    m_pTemp3   = temp3;
    m_TimeNum  = tnum;
    m_FreqNum  = fnum;
    int half = tnum >> 1;
    int mask = tnum -  1;
    float base = (tmax - tmin) / tnum;
    float step = 1.0f / (fnum - 1);
    {
        auto in  = reinterpret_cast<fftwf_complex *>(temp1);
        auto out = reinterpret_cast<fftwf_complex *>(temp2);
        fftwf_plan_with_nthreads(omp_get_max_threads());
        fft = fftwf_plan_dft_1d(fnum, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    }
    for(size_t f = 0; f < fnum; f++){
        float scale = base * Interpolation(fmin, fmax, f * step);
        float norm  = 2.0f * scale;
        for(int t = -half; t < half; t++) {
            temp1[t & mask] = norm * MotherWavelet(t * scale);
        }
        fftwf_execute(fft);
        memcpy(table, temp2, tnum * sizeof(complex<float>));
        table += tnum;
    }
    fftwf_destroy_plan(fft);
    return true;
}

bool Wavelet::Exec(float *dst, const float *src) {
    if(!dst || !src) return false;
    auto fft   = m_PlanFFT;
    auto ifft  = m_PlanIFFT;
    auto table = m_pTable;
    auto temp0 = m_pTemp0;
    auto temp1 = m_pTemp1;
    auto temp2 = m_pTemp2;
    auto temp3 = m_pTemp3;
    auto tnum  = m_TimeNum;
    auto fnum  = m_FreqNum;
    memcpy(temp0, src, tnum * sizeof(float));
    fftwf_execute(fft);
    for(size_t f = 0; f < fnum; f++, table += tnum, dst += tnum){
        for(size_t t = 0; t < tnum; t++) temp2[t] = table[t] * temp1[t];
        fftwf_execute(ifft);
        for(size_t t = 0; t < tnum; t++) dst[t] = std::abs(temp3[t]);
    }
    return true;
}

}  // namespace WaveletAnaryzer
