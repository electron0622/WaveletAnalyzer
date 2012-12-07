//============================================================================
//
//  audio/file/reader.hpp
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

#ifndef _AUDIO_FILE_READER_HPP_
#define _AUDIO_FILE_READER_HPP_

#include <queue>
#include <thread>
#include <mutex>
#include "ffmpegwrapper.hpp"
#include "../../util/io.hpp"

namespace WaveletAnalyzer {

namespace Audio {

namespace File {

using std::queue;
using std::thread;
using std::mutex;

class Reader : protected FFmpegWrapper, public Util::IO {

public:
    Reader();
    ~Reader();

public:
    bool SetCacheSize(size_t size);
    bool SetSampleRate(size_t rate);
    bool SetNumChannels(size_t ch);

public:
    size_t GetCacheSize(void) const;
    size_t GetSampleRate(void) const;
    size_t GetNumChannels(void) const;

public:
    bool   Open(const char *name);
    void   Close(void);
    size_t Read(void *data, size_t size);
    bool   Seek(size_t offset);
    size_t Tell(void);

private:
    void Main(void);

private:
    template<class T>
    void FillBuffer(T bias, float scale);

private:
    queue<float> m_Buffer;
    thread       m_Thread;
    mutex        m_Mutex;
    SampleFormat m_SampleFormat;
    size_t       m_MaxBufNum;
    size_t       m_ReadPos;
    bool         m_EndFlag;

};

}  // namespace File

}  // namespace Audio

}  // namespace WaveletAnaryzer

#endif /* _AUDIO_FILE_READER_HPP_ */
