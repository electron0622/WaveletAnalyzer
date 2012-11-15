//============================================================================
//
//  audio/file/ffmpegwrapper.hpp
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

#ifndef _AUDIO_FILE_FFMPEGWRAPPER_HPP_
#define _AUDIO_FILE_FFMPEGWRAPPER_HPP_

#include <mutex>
#include "../format.hpp"

struct AVFormatContext;
struct AVCodecContext;
struct AVPacket;
struct AVFrame;

namespace WaveletAnalyzer {

namespace Audio {

namespace File {

using std::call_once;
using std::once_flag;

class FFmpegWrapper {

public:
    FFmpegWrapper();
    virtual ~FFmpegWrapper();

public:
    size_t GetSampleRate(void) const;
    size_t GetNumChannels(void) const;

public:
    bool   Open(const char *name, SampleFormat &sfmt);
    void   Close(void);
    size_t Read(void *data, size_t size);
    bool   Seek(size_t offset);

private:
    template<class T>
    size_t Decode(T *data, size_t size);

private:
    AVFormatContext *m_pFormatContext;
    AVCodecContext  *m_pCodecContext;
    AVPacket        *m_pPacket;
    AVFrame         *m_pFrame;
    unsigned int     m_StreamIndex;

private:
    static once_flag m_InitFlag;

};

}  // namespace File

}  // namespace Audio

}  // namespace WaveletAnaryzer

#endif /* _AUDIO_FILE_FFMPEGWRAPPER_HPP_ */
