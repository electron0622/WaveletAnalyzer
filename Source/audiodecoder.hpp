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

#include <thread>
#include <mutex>
#include <queue>
#include "audioreader.hpp"
#include "media.hpp"

namespace WaveletAnalyzer {

using std::thread;
using std::mutex;
using std::queue;

class AudioDecoder : public AudioReader, public Media {

public:
    AudioDecoder(const char *path);
    ~AudioDecoder();

public:
    bool   IsOpen(void);
    void   SeekSet(void);
    size_t Read(float *data, size_t num);
    size_t GetNumChannels(void);
    size_t GetSampleRate(void);

private:
    void Main(void);

private:
    thread       m_Thread;
    mutex        m_Mutex;
    queue<float> m_Buffer;
    bool         m_OpenFlag;
    bool         m_SeekFlag;

};

}  // namespace WaveletAnalyzer

#endif /* _AUDIODECODER_HPP_ */
