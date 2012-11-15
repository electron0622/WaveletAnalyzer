//============================================================================
//
//  audio/file/ffmpegwrapper.cpp
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
#include <libavcodec/avcodec.h>
}
#include <queue>
#include "ffmpegwrapper.hpp"

namespace WaveletAnalyzer {

namespace Audio {

namespace File {

using std::queue;

FFmpegWrapper::FFmpegWrapper() : m_pFormatContext(nullptr),
        m_pCodecContext(nullptr), m_pPacket(nullptr),
        m_pFrame(nullptr), m_StreamIndex(0) {
    auto init = [](void) {
        av_register_all();
        return;
    };
    call_once(m_InitFlag, init);
}

FFmpegWrapper::~FFmpegWrapper() {
    Close();
}

size_t FFmpegWrapper::GetSampleRate(void) const {
    return (size_t)m_pCodecContext->sample_rate;
}

size_t FFmpegWrapper::GetNumChannels(void) const {
    return (size_t)m_pCodecContext->channels;
}

bool FFmpegWrapper::Open(const char *name, SampleFormat &sfmt) {
    if(!name) return false;
    Close();
    auto ppfc = &m_pFormatContext;
    if(avformat_open_input(ppfc, name, nullptr, nullptr)) return false;
    auto pfc = *ppfc;
    if(avformat_find_stream_info(pfc, nullptr) < 0) return false;
    AVStream *stream = nullptr;
    auto nb_streams = pfc->nb_streams;
    for(unsigned int i = 0; i < nb_streams; ++i) {
        auto s = pfc->streams[i];
        if(s->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            stream        = s;
            m_StreamIndex = i;
            break;
        }
    }
    if(!stream) return false;
    auto codec = avcodec_find_decoder(stream->codec->codec_id);
    if(!codec) return false;
    auto pcc = avcodec_alloc_context3(codec);
    m_pCodecContext = pcc;
    if(!pcc || avcodec_open2(pcc, codec, nullptr) < 0) return false;
    auto ppacket = (AVPacket *)av_malloc(sizeof(AVPacket));
    m_pPacket = ppacket;
    if(!ppacket) return false;
    av_init_packet(ppacket);
    auto pframe = avcodec_alloc_frame();
    m_pFrame = pframe;
    if(!pframe) return false;
    avcodec_get_frame_defaults(pframe);
    sfmt.SampleRate  = pcc->sample_rate;
    sfmt.NumChannels = pcc->channels;
    switch(pcc->sample_fmt) {
    case AV_SAMPLE_FMT_U8:  sfmt.FormatId = BF_U8;      break;
    case AV_SAMPLE_FMT_S16: sfmt.FormatId = BF_S16;     break;
    case AV_SAMPLE_FMT_S32: sfmt.FormatId = BF_S32;     break;
    case AV_SAMPLE_FMT_FLT: sfmt.FormatId = BF_F32;     break;
    case AV_SAMPLE_FMT_DBL: sfmt.FormatId = BF_F64;     break;
    default:                sfmt.FormatId = BF_UNKNOWN; break;
    }
    return true;
}

void FFmpegWrapper::Close(void) {
    auto ppframe = &m_pFrame;
    avcodec_free_frame(ppframe);
    auto pppacket = &m_pPacket;
    av_freep(pppacket);
    auto ppcc = &m_pCodecContext;
    if(*ppcc) avcodec_close(*ppcc);
    av_freep(ppcc);
    auto ppfc = &m_pFormatContext;
    if(*ppfc) avformat_close_input(ppfc);
    return;
}

size_t FFmpegWrapper::Read(void *data, size_t size) {
    if(!data) return 0;
    auto pcc = m_pCodecContext;
    switch(pcc->sample_fmt) {
    case AV_SAMPLE_FMT_U8:   size = Decode((uint8_t *)data, size); break;
    case AV_SAMPLE_FMT_S16:  size = Decode((int16_t *)data, size); break;
    case AV_SAMPLE_FMT_S32:  size = Decode((int32_t *)data, size); break;
    case AV_SAMPLE_FMT_FLT:  size = Decode((float   *)data, size); break;
    case AV_SAMPLE_FMT_DBL:  size = Decode((double  *)data, size); break;
    default:                 size = 0;                             break;
    }
    return size;
}

bool FFmpegWrapper::Seek(size_t offset) {
    return false;
}

template<class T>
size_t FFmpegWrapper::Decode(T *data, size_t size) {
    static queue<T> buf;
    auto pfc      = m_pFormatContext;
    auto pcc      = m_pCodecContext;
    auto ppacket  = m_pPacket;
    auto pframe   = m_pFrame;
    auto index    = m_StreamIndex;
    int  finished = 0;
    size /= sizeof(T);
    while(buf.size() < size) {
        if(av_read_frame(pfc, ppacket) < 0) {
            size = buf.size();
            break;
        }
        if(ppacket->stream_index == (int)index) {
            avcodec_decode_audio4(pcc, pframe, &finished, ppacket);
            if(finished) {
                T  *ptr = (T *)pframe->extended_data[0];
                int num = pframe->linesize[0] / sizeof(T);
                for(int i = 0; i < num; i++) buf.push(ptr[i]);
            }
        }
        av_free_packet(ppacket);
    }
    {
        T *ptr = (T *)data;
        for(size_t i = 0; i < size; i++) {
            ptr[i] = buf.front();
            buf.pop();
        }
    }
    size *= sizeof(T);
    return size;
}

once_flag FFmpegWrapper::m_InitFlag;

}  // namespace File

}  // namespace Audio

}  // namespace WaveletAnaryzer
