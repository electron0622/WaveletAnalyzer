//============================================================================
//
//  media.hpp
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

#ifndef _MEDIA_HPP_
#define _MEDIA_HPP_

#include <mutex>

struct AVFormatContext;
struct AVCodecContext;

namespace WaveletAnalyzer {

using std::call_once;
using std::once_flag;

class Media {

public:
    Media();
    virtual ~Media();

protected:
    AVFormatContext *m_pFormatContext;
    AVCodecContext  *m_pCodecContext;

private:
    static once_flag m_InitFlag;

};

}  // namespace WaveletAnalyzer

#endif /* _MEDIA_HPP_ */
