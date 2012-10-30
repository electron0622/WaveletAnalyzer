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

#include "ffmpegwrapper.hpp"

namespace WaveletAnalyzer {

namespace Audio {

namespace File {

template<class _OUT, class _IN>
static void convert(_OUT *dst, _IN *src, _OUT scale, size_t num) {
    for(size_t i = 0; i < num; i++) dst[i] = src[i] * scale;
    return;
}

FFmpegWrapper::FFmpegWrapper() : m_pFormatContext(nullptr),
        m_pCodecContext(nullptr), m_Index(0) {
    auto init = [](void) {
        av_register_all();
        return;
    };
    call_once(m_InitFlag, init);
}

FFmpegWrapper::~FFmpegWrapper() {
    Close();
}

bool FFmpegWrapper::Open(const char *name, SampleFormat *psfmt) {
    if(!name || !psfmt) return false;
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
    m_pPacket = (AVPacket *)av_malloc(sizeof(AVPacket));
    if(!m_pPacket) return false;
    av_init_packet(m_pPacket);
    m_pFrame = avcodec_alloc_frame();
    if(!m_pFrame) return false;
    avcodec_get_frame_defaults(m_pFrame);
    m_pSampleFmt = psfmt;
    m_ReadPos    = 0;
    m_Finished   = 0;
    return true;
}

void FFmpegWrapper::Close(void) {
    auto frame = &m_pFrame;
    avcodec_free_frame(frame);
    auto pkt = m_pPacket;
    if(pkt) {
        av_free(pkt);
        m_pPacket = nullptr;
    }
    auto pcc = m_pCodecContext;
    if(pcc) {
        avcodec_close(pcc);
        av_free(pcc);
        m_pPacket = nullptr;
    }
    auto ppfc = &m_pFormatContext;
    avformat_close_input(ppfc);
    return;
}

size_t FFmpegWrapper::Read(void *data, size_t size) {
    auto pfc      = m_pFormatContext;
    auto pcc      = m_pCodecContext;
    auto ppkt     = m_pPacket;
    auto pframe   = m_pFrame;
    auto psfmt    = m_pSampleFmt;
    auto pos      = m_ReadPos;
    auto finished = m_Finished;
    auto index    = m_StreamIndex;
    size_t i = 0;
    while(i < size) {
        while(!pos) {
            if(av_read_frame(pfc, ppkt) < 0) {
                m_ReadPos  = pos;
                m_Finished = finished;
                return i;
            }
            if(ppkt->stream_index == index) {
                avcodec_decode_audio4(pcc, pframe, &finished, ppkt);
                if(finished) break;
            }
        }
        if(psfmt) {
            psfmt->SampleRate  = pcc->sample_rate;
            psfmt->NumChannels = pcc->channels;
            switch(pcc->sample_fmt) {
            case AV_SAMPLE_FMT_U8:
                psfmt->FormatId = BF_U8;
                break;
            case AV_SAMPLE_FMT_S16:
                psfmt->FormatId = BF_S16;
                break;
            case AV_SAMPLE_FMT_S32:
                psfmt->FormatId = BF_S32;
                break;
            case AV_SAMPLE_FMT_FLT:
                psfmt->FormatId = BF_F32;
                break;
            case AV_SAMPLE_FMT_DBL:
                psfmt->FormatId = BF_F64;
                break;
            default:
                psfmt->FormatId = BF_VOID;
                break;
            }
            m_pSampleFmt = nullptr;
        }
            {
                double *data = (double *)(frame.extended_data[0]);
                size_t size = frame.linesize[0] >> 3;
                for(size_t i = 0; i < size; i++) {
                    m_Buffer.push_back(data[i]);
                }
            }
    }
    return size;
}

bool FFmpegWrapper::Seek(size_t offset) {
    return false;
}

size_t FFmpegWrapper::Tell(void) {
    return 0;
}

once_flag FFmpegWrapper::m_InitFlag;

}  // namespace File

}  // namespace Audio

}  // namespace WaveletAnaryzer
