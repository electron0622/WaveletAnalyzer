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

Wavelet::Wavelet() : m_pPlan(nullptr), m_pTable(nullptr),
        m_pTemp0(nullptr), m_pTemp1(nullptr),
        m_pTemp2(nullptr), m_pTemp3(nullptr),
        m_TimeNum(0), m_FreqNum(0) {
}

Wavelet::~Wavelet() {
    auto plan = m_pPlan;
    auto fnum = m_FreqNum;
    if(plan) for(size_t i = 0; i <= fnum; i++) fftwf_destroy_plan(plan[i]);
    delete[] m_pPlan;
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
    auto plan = m_pPlan;
    if(plan) for(size_t i = 0; i <= fnum; i++) fftwf_destroy_plan(plan[i]);
    delete[] m_pPlan;  m_pPlan  = nullptr;
    delete[] m_pTable; m_pTable = nullptr;
    delete[] m_pTemp0; m_pTemp0 = nullptr;
    delete[] m_pTemp1; m_pTemp1 = nullptr;
    delete[] m_pTemp2; m_pTemp2 = nullptr;
    delete[] m_pTemp3; m_pTemp3 = nullptr;
    auto *table = new complex<float>[tnum * fnum];
    auto *temp0 = new float         [tnum       ];
    auto *temp1 = new complex<float>[tnum       ];
    auto *temp2 = new complex<float>[tnum * fnum];
    auto *temp3 = new complex<float>[tnum * fnum];
    plan = new fftwf_plan[fnum + 1];
    for(size_t i = 0; i < fnum; i++) {
        constexpr auto mode = FFTW_BACKWARD;
        constexpr auto flag = FFTW_ESTIMATE;
        auto pos = tnum * i;
        auto src = reinterpret_cast<fftwf_complex *>(temp2 + pos);
        auto dst = reinterpret_cast<fftwf_complex *>(temp3 + pos);
        plan[i]  = fftwf_plan_dft_1d(fnum, src, dst, mode, flag);
    }
    {
        constexpr auto flag = FFTW_ESTIMATE;
        auto src   = reinterpret_cast<float         *>(temp0);
        auto dst   = reinterpret_cast<fftwf_complex *>(temp1);
        plan[fnum] = fftwf_plan_dft_r2c_1d(fnum, src, dst, flag);
    }
    m_pPlan    = plan;
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
    for(size_t i = 0; i < fnum; i++) {
        constexpr auto mode = FFTW_FORWARD;
        constexpr auto flag = FFTW_ESTIMATE;
        auto pos  = tnum * i;
        auto tmp  = temp2 + pos;
        auto src  = reinterpret_cast<fftwf_complex *>(tmp);
        auto dst  = reinterpret_cast<fftwf_complex *>(table + pos);
        auto plan = fftwf_plan_dft_1d(fnum, src, dst, mode, flag);
        float xscale = base * Interpolation(fmin, fmax, i * step);
        float yscale = 2.0f * xscale;
        for(int j = -half; j < half; j++) {
            tmp[j & mask] = yscale * MotherWavelet(j * xscale);
        }
        fftwf_execute(plan);
        fftwf_destroy_plan(plan);
    }
    return true;
}

bool Wavelet::Exec(float *dst, const float *src) {
    if(!dst || !src) return false;
    auto plan  = m_pPlan;
    auto table = m_pTable;
    auto temp0 = m_pTemp0;
    auto temp1 = m_pTemp1;
    auto temp2 = m_pTemp2;
    auto temp3 = m_pTemp3;
    auto tnum  = m_TimeNum;
    auto fnum  = m_FreqNum;
    memcpy(temp0, src, tnum * sizeof(float));
    fftwf_execute(plan[fnum]);
#pragma omp parallel for
    for(size_t i = 0; i < fnum; i++) {
        auto pos = tnum * i;
        auto t0  = table + pos;
        auto t1  = temp1;
        auto t2  = temp2 + pos;
        auto t3  = temp3 + pos;
        auto t4  = dst   + pos;
        for(size_t j = 0; j < tnum; j++) t2[j] = t0[j] * t1[j];
        fftwf_execute(plan[i]);
        for(size_t j = 0; j < tnum; j++) t4[j] = std::abs(t3[j]);
    }
    return true;
}

}  // namespace WaveletAnaryzer
