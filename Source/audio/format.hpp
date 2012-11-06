//============================================================================
//
//  audio/format.hpp
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

#ifndef _AUDIO_FORMAT_HPP_
#define _AUDIO_FORMAT_HPP_

namespace WaveletAnalyzer {

namespace Audio {

enum BitFormat {
    BF_UNKNOWN,
    BF_S8,
    BF_U8,
    BF_S16,
    BF_U16,
    BF_S32,
    BF_U32,
    BF_F32,
    BF_F64
};

struct SampleFormat {
    size_t    SampleRate;
    size_t    NumChannels;
    BitFormat FormatId;
};

}  // namespace Audio

}  // namespace WaveletAnaryzer

#endif /* _AUDIO_FORMAT_HPP_ */
