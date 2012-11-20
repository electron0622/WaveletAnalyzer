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

#include <plplot/plstream.h>

namespace WaveletAnalyzer {

namespace Plot {

class Line {

public:
    Line();
    ~Line();

private:
    plstream *m_pStream;

};

}  // namespace Plot

}  // namespace WaveletAnalyzer

#endif /* _PLOT_LINE_HPP_ */
