//============================================================================
//
//  main.hpp
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

#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <wx/app.h>

namespace WaveletAnalyzer {

class MainApp : public wxApp {

public:
    bool OnInit(void);

};

}  // namespace WaveletAnalyzer

#endif /* _MAIN_HPP_ */
