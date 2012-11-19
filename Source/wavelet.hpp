//============================================================================
//
//  wavelet.hpp
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

#ifndef _WAVELET_HPP_
#define _WAVELET_HPP_

#include <fftw3.h>
#include <complex>
#include <functional>

namespace WaveletAnalyzer {

using std::complex;
using std::function;

typedef function<complex<float> (float)>      MotherWaveletFunc;
typedef function<float (float, float, float)> InterpolationFunc;

class Wavelet {

public:
    Wavelet();
    ~Wavelet();

public:
    bool Init(float tmin, float tmax, size_t tnum,
              float fmin, float fmax, size_t fnum,
              MotherWaveletFunc &MotherWavelet,
              InterpolationFunc &Interpolation);

public:
    bool Exec(float *dst, const float *src);

private:
    fftwf_plan      m_PlanFFT;
    fftwf_plan      m_PlanIFFT;
    complex<float> *m_pTable;
    float          *m_pTemp0;
    complex<float> *m_pTemp1;
    complex<float> *m_pTemp2;
    complex<float> *m_pTemp3;
    size_t          m_TimeNum;
    size_t          m_FreqNum;

};

}  // namespace WaveletAnalyzer

#endif /* _WAVELET_HPP_ */
