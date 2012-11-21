//============================================================================
//
//  line.hpp
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

#ifndef _PLOT_LINE_HPP_
#define _PLOT_LINE_HPP_

#include <stdint.h>
#include <functional>
#include <complex>

namespace WaveletAnalyzer {

namespace Plot {

using std::function;
using std::complex;

typedef function<complex<float> (float)> LineFunc;

class Line {

public:
    Line();
    ~Line();

public:
    bool Init(size_t width, size_t height);

public:
    size_t GetWidth(void) const;
    size_t GetHeight(void) const;
    const void *GetData(void) const;

public:
    void SetRange(float xmin, float xmax, float ymin, float ymax);

public:
    void Draw(LineFunc *func, size_t width, size_t height);

private:
    uint8_t *m_pData;
    size_t   m_Width;
    size_t   m_Height;
    float    m_MinX;
    float    m_MaxX;
    float    m_MinY;
    float    m_MaxY;

};

}  // namespace Plot

}  // namespace WaveletAnalyzer

#endif /* _PLOT_LINE_HPP_ */
