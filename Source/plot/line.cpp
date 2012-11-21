//============================================================================
//
//  line.cpp
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
#include "line.hpp"

namespace WaveletAnalyzer {

namespace Plot {

Line::Line() : m_pData(nullptr), m_Width(0), m_Height(0) {
}

Line::~Line() {
    free(m_pData);
}

bool Line::Init(size_t width, size_t height) {
    free(m_pData);
    m_pData = malloc(width * height * 8);
    if(!m_pData) return false;
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

void Line::Draw(size_t width, size_t height) {
    plstream pls;
    pls.sdev("mem");
    pls.smem(m_Width, m_Height, m_pData);
    pls.init();
    pls.vpas(0.0, width, 0.0, height, 1.0);
    pls.wind(-1.3, 1.3, -1.3, 1.3);
    pls.box("a", 0, 0, "a", 0, 0);
    return;
}

}  // namespace Plot

}  // namespace WaveletAnalyzer
