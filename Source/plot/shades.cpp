//============================================================================
//
//  plot/shades.cpp
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

#include <string.h>
#include <plplot/plstream.h>
#include <vector>
#include <limits>
#include "shades.hpp"

namespace WaveletAnalyzer {

namespace Plot {

using std::vector;
using std::numeric_limits;
using std::isnan;

Shades::Shades() : m_pColorData(nullptr), m_pFloatData(nullptr),
        m_Width(0), m_Height(0),
        m_MinX(-1.0f), m_MaxX(1.0f), m_MinY(-1.0f), m_MaxY(1.0f) {
}

Shades::~Shades() {
    delete[] m_pColorData;
    delete[] m_pFloatData;
}

bool Shades::Init(size_t width, size_t height) {
    delete[] m_pColorData;
    delete[] m_pFloatData;
    m_pColorData = new uint8_t[width * height * 3];
    m_pFloatData = new float  [width * height    ];
    m_Width  = width;
    m_Height = height;
    return true;
}

size_t Shades::GetWidth(void) const {
    return m_Width;
}

size_t Shades::GetHeight(void) const {
    return m_Height;
}

const void *Shades::GetData(void) const {
    return m_pColorData;
}

float Shades::GetX(size_t x, size_t width) const {
    auto xmingap = XGAP;
    auto xmaxgap = width - xmingap;
    if(x < xmingap || x > xmaxgap) return numeric_limits<float>::quiet_NaN();
    x     -= xmingap;
    width -= xmingap << 1;
    auto alpha = (float)x / (width - 1);
    return m_MinX * (1.0f - alpha) + m_MaxX * alpha;
}

float Shades::GetY(size_t y, size_t height) const {
    auto ymingap = YGAP;
    auto ymaxgap = height - ymingap;
    if(y < ymingap || y > ymaxgap) return numeric_limits<float>::quiet_NaN();
    y      -= ymingap;
    height -= ymingap << 1;
    auto alpha  = 1.0f - (float)y / (height - 1);
    return std::pow(10, (m_MaxY - m_MinY) * alpha + m_MinY);
}

float Shades::GetZ(size_t x, size_t y) const {
    auto width  = m_Width;
    auto height = m_Height;
    if(x >= width || y >= height) return numeric_limits<float>::quiet_NaN();
    return m_pFloatData[x + y * width];
}

void Shades::SetRange(float xmin, float xmax, float ymin, float ymax) {
    m_MinX = xmin;
    m_MaxX = xmax;
    m_MinY = std::log10(ymin);
    m_MaxY = std::log10(ymax);
    return;
}

void Shades::Draw(const float_ptr *data, size_t xnum, size_t ynum,
                  size_t width, size_t height) {
    auto mem1 = m_pColorData;
    auto mem2 = m_pFloatData;
    auto wmax = m_Width;
    auto hmax = m_Height;
    memset(mem1, 0, wmax * hmax * 3);
    memset(mem2, 0, wmax * hmax * sizeof(float));
    plstream pls;
    pls.sdev("mem");
    pls.smem(wmax, hmax, mem1);
    pls.init();
    pls.adv(0);
    pls.schr(2, 1.0f);
    pls.col0(3);
    auto xmingap = XGAP;
    auto ymingap = YGAP;
    auto xmaxgap = width  - xmingap;
    auto ymaxgap = height - ymingap;
    auto xmin =        (float)xmingap / wmax;
    auto xmax =        (float)xmaxgap / wmax;
    auto ymin = 1.0f - (float)ymaxgap / hmax;
    auto ymax = 1.0f - (float)ymingap / hmax;
    if(data) {
        Lerp(mem1,    mem2,    wmax,    hmax,
             xmingap, xmaxgap, ymingap, ymaxgap,
             data,    xnum,    ynum);
    }
    pls.vpor(xmin, xmax, ymin, ymax);
    xmin = m_MinX;
    xmax = m_MaxX;
    ymin = m_MinY;
    ymax = m_MaxY;
    pls.wind(xmin, xmax, ymin, ymax);
    pls.box("bcfgnt", 0.0f, 0, "bcfglntv", 0.0f, 0);
    pls.lab("Time[sec]", "Frequency[Hz]", "Spectrum");
    return;
}

void Shades::Lerp(uint8_t *dst1, float *dst2, size_t dstw, size_t dsth,
                  size_t xmin, size_t xmax, size_t ymin, size_t ymax,
                  const float_ptr *src, size_t srcw, size_t srch) {
    auto xscale = (float)srcw / (xmax - xmin);
    auto yscale = (float)srch / (ymax - ymin);
    for(size_t i = 0, x = xmin + 1; x < xmax; i++, x++) {
        auto xsrcf  = i * xscale;
        auto xsrci  = (size_t)xsrcf;
        auto xalpha = xsrcf - xsrci;
        auto x1 = src[xsrci    ].get();
        auto x2 = src[xsrci + 1].get();
        for(size_t j = 0, y = ymax - 1; y > ymin; j++, y--) {
            auto ysrcf  = j * yscale;
            auto ysrci  = (size_t)ysrcf;
            auto yalpha = ysrcf - ysrci;
            auto y1 = x1[ysrci] * (1.0f - yalpha) + x1[ysrci + 1] * yalpha;
            auto y2 = x2[ysrci] * (1.0f - yalpha) + x2[ysrci + 1] * yalpha;
            auto gray = y1 * (1.0f - xalpha) + y2 * xalpha;
            auto offset = x + y * dstw;
            UnormToRGB(dst1 + offset * 3, dst2[offset] = gray);
        }
    }
    return;
}

void Shades::UnormToRGB(uint8_t *dst, float src) {
    float  c4 = std::max(std::min(src, 1.0f), 0.0f) * 6.0f;
    size_t hi = c4;
    size_t f  = (c4 - hi) * 0xff;
    switch(hi) {
    case 0:  dst[0] = 0x00; dst[1] = 0x00;     dst[2] = f;        break;
    case 1:  dst[0] = 0x00; dst[1] = f;        dst[2] = 0xff;     break;
    case 2:  dst[0] = 0x00; dst[1] = 0xff;     dst[2] = 0xff - f; break;
    case 3:  dst[0] = f;    dst[1] = 0xff;     dst[2] = 0x00;     break;
    case 4:  dst[0] = 0xff; dst[1] = 0xff - f; dst[2] = 0x00;     break;
    case 5:  dst[0] = 0xff; dst[1] = f;        dst[2] = f;        break;
    case 6:  dst[0] = 0xff; dst[1] = 0xff;     dst[2] = 0xff;     break;
    default:                                                      break;
    }
    return;
}

}  // namespace Plot

}  // namespace WaveletAnalyzer
