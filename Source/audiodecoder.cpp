//============================================================================
//
//  audiodecoder.cpp
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
#include "audiodecoder.hpp"

namespace WaveletAnalyzer {

AudioDecoder::AudioDecoder() {
}

AudioDecoder::~AudioDecoder() {
}

bool AudioDecoder::Open(const char *path) {
    auto ppFmtCtx = &m_pFormatContext;
    if(avformat_open_input(ppFmtCtx, path, nullptr, nullptr)) return false;
    auto pFmtCtx = *ppFmtCtx;
    if(avformat_find_stream_info(pFmtCtx, nullptr) < 0) return false;
    AVStream *stream = nullptr;
    auto nb_streams = pFmtCtx->nb_streams;
    for(auto i = 0U; i < nb_streams; i++) {
        stream = pFmtCtx->streams[i];
        if(stream->codec->codec_type == AVMEDIA_TYPE_VIDEO) break;
    }
    if(!stream) return false;
    auto codec = avcodec_find_decoder(stream->codec->codec_id);
    m_pCodecContext = avcodec_alloc_context3(codec);
    if(!m_pCodecContext) return false;
    if(avcodec_open2(m_pCodecContext, codec, nullptr) < 0) return false;
//    avcodec_decode_audio4(m_pCodecContext, frame, &frameFinished, packet);
    return true;
}

void AudioDecoder::Close(void) {
    avcodec_close(m_pCodecContext);
    av_free(m_pCodecContext);
    avformat_close_input(&m_pFormatContext);
    return;
}

}  // namespace WaveletAnalyzer
