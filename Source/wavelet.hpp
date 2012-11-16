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

#include <complex>
#include <functional>

namespace WaveletAnalyzer {

using std::complex;
using std::function;

typedef function<complex<float> (float)> WaveletFunc;

class Wavelet {

public:
    Wavelet();
    ~Wavelet();

public:
    bool Init(WaveletFunc &func, size_t nx, size_t ny, float hf, float lf);

public:
    bool Exec(complex<float> *dst, float *src);

private:
    WaveletFunc m_Function;

};

}  // namespace WaveletAnalyzer

#endif /* _WAVELET_HPP_ */
