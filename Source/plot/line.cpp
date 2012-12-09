//============================================================================
//
//  plot/line.cpp
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

#include <plplot/plstream.h>
#include <cstring>
#include <vector>
#include "line.hpp"

namespace WaveletAnalyzer {

namespace Plot {

using std::memset;
using std::vector;

Line::Line() : m_pData(nullptr), m_Width(0), m_Height(0),
        m_MinX(-1.0f), m_MaxX(1.0f), m_MinY(-1.0f), m_MaxY(1.0f) {
}

Line::~Line() {
    delete[] m_pData;
}

bool Line::Init(size_t width, size_t height) {
    delete[] m_pData;
    m_pData  = new uint8_t[width * height * 3];
    m_Width  = width;
    m_Height = height;
    return true;
}

size_t Line::GetWidth(void) const {
    return m_Width;
}

size_t Line::GetHeight(void) const {
    return m_Height;
}

const void *Line::GetData(void) const {
    return m_pData;
}

void Line::SetRange(float xmin, float xmax, float ymin, float ymax) {
    m_MinX = xmin;
    m_MaxX = xmax;
    m_MinY = ymin;
    m_MaxY = ymax;
    return;
}

void Line::Draw(LineFunc *func, size_t width, size_t height) {
    auto mem  = m_pData;
    auto wmax = m_Width;
    auto hmax = m_Height;
    memset(mem, 0, wmax * hmax * 3);
    plstream pls;
    pls.sdev("mem");
    pls.smem(wmax, hmax, mem);
    pls.init();
    pls.adv(0);
    pls.schr(2, 1.0f);
    pls.col0(3);
    auto xgap = 40.0f;
    auto ygap = 30.0f;
    auto xmin =                  xgap  / wmax;
    auto xmax =        (width  - xgap) / wmax;
    auto ymin = 1.0f - (height - ygap) / hmax;
    auto ymax = 1.0f -           ygap  / hmax;
    pls.vpor(xmin, xmax, ymin, ymax);
    xmin = m_MinX;
    xmax = m_MaxX;
    ymin = m_MinY;
    ymax = m_MaxY;
    pls.wind(xmin, xmax, ymin, ymax);
    pls.box("abcfgnt", 0.0f, 0, "abcfgntv", 0.0f, 0);
    pls.lab("", "", "Mother Wavelet");
    if(!func) return;
    int num = width - 2.0f * xgap;
    if(num < 2) return;
    static vector<float> xr;
    static vector<float> yr;
    static vector<float> yi;
    xr.resize(num);
    yr.resize(num);
    yi.resize(num);
    for(int i = 0; i < num; i++) {
        auto a = (float)i / (num - 1);
        xr[i]  = xmin * (1.0f - a) + xmax * a;
        auto y = (*func)(xr[i]);
        yr[i]  = y.real();
        yi[i]  = y.imag();
    }
    pls.col0(9);
    pls.line(num, &xr[0], &yi[0]);
    pls.col0(1);
    pls.line(num, &xr[0], &yr[0]);
    return;
}

}  // namespace Plot

}  // namespace WaveletAnalyzer
