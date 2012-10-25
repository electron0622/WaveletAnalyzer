//============================================================================
//
//  media.cpp
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

#define __STDC_CONSTANT_MACROS
extern "C" {
#include <libavformat/avformat.h>
}
#include "media.hpp"

namespace WaveletAnalyzer {

Media::Media() {
    call_once(m_InitFlag, [](){ av_register_all(); });
}

Media::~Media() {
}

once_flag Media::m_InitFlag;

}  // namespace WaveletAnalyzer
