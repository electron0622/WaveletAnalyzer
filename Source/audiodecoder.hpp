//============================================================================
//
//  audiodecoder.hpp
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

#ifndef _AUDIODECODER_HPP_
#define _AUDIODECODER_HPP_

#include <queue>
#include "media.hpp"

namespace WaveletAnalyzer {

using std::queue;

struct AudioInfo {
    int ch;
    int bits;
    int samples;
};

class AudioDecoder : public Media {

public:
    AudioDecoder();
    ~AudioDecoder();

public:
    bool   Open(const char *path);
    void   Close(void);
    size_t Read(int *data, size_t num);
    size_t Seek(size_t offset);
    bool   GetInfo(AudioInfo *info);

private:
    queue<int> m_RingBuffer;

};

}  // namespace WaveletAnalyzer

#endif /* _AUDIODECODER_HPP_ */
