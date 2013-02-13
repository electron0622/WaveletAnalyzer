//============================================================================
//
//  plot/shades.hpp
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

#ifndef _PLOT_SHADES_HPP_
#define _PLOT_SHADES_HPP_

#include <stdint.h>
#include <memory>
#include <functional>
#include <complex>

namespace WaveletAnalyzer {

namespace Plot {

using std::shared_ptr;
using std::function;
using std::complex;

typedef shared_ptr<float> float_ptr;

class Shades {

public:
    Shades();
    ~Shades();

public:
    bool Init(size_t width, size_t height);

public:
    size_t GetWidth(void) const;
    size_t GetHeight(void) const;
    const void *GetData(void) const;

public:
    float GetX(size_t x, size_t width) const;
    float GetY(size_t y, size_t height) const;
    float GetZ(size_t x, size_t y) const;

public:
    void SetRange(float xmin, float xmax, float ymin, float ymax);

public:
    void Draw(const float_ptr *data, size_t xnum, size_t ynum,
              size_t width, size_t height);

private:
    void Lerp(uint8_t *dst1, float *dst2, size_t dstw, size_t dsth,
              size_t xmin, size_t xmax, size_t ymin, size_t ymax,
              const float_ptr *src, size_t srcw, size_t srch);

private:
    inline void UnormToRGB(uint8_t *dst, float src);

private:
    const size_t XGAP = 70;
    const size_t YGAP = 40;

private:
    uint8_t *m_pColorData;
    float   *m_pFloatData;
    size_t   m_Width;
    size_t   m_Height;
    float    m_MinX;
    float    m_MaxX;
    float    m_MinY;
    float    m_MaxY;

};

}  // namespace Plot

}  // namespace WaveletAnalyzer

#endif /* _PLOT_SHADES_HPP_ */
