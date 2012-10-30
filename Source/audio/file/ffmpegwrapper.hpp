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

#define __STDC_CONSTANT_MACROS
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
#include <mutex>
#include "../format.hpp"

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
    bool   Open(const char *name, SampleFormat &sfmt);
    void   Close(void);
    size_t Read(void *data, size_t size);
    bool   Seek(size_t offset);
    size_t Tell(void);

private:
    AVFormatContext *m_pFormatContext;
    AVCodecContext  *m_pCodecContext;
    AVPacket        *m_pPacket;
    AVFrame         *m_pFrame;
    SampleFormat    *m_pSampleFmt;
    size_t           m_ReadPos;
    int              m_Finished;
    unsigned int     m_StreamIndex;

private:
    static once_flag m_InitFlag;

};

}  // namespace File

}  // namespace Audio

}  // namespace WaveletAnaryzer

#endif /* _AUDIO_FILE_FFMPEGWRAPPER_HPP_ */
