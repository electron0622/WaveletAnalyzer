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
#include <unistd.h>
#include <functional>
#include "audiodecoder.hpp"

namespace WaveletAnalyzer {

using std::bind;

AudioDecoder::AudioDecoder(const char *path) : m_OpenFlag(false), m_SeekFlag(false) {
    auto ppfc = &m_pFormatContext;
    if(avformat_open_input(ppfc, path, nullptr, nullptr)) return;
    auto pfc = *ppfc;
    if(avformat_find_stream_info(pfc, nullptr) < 0) return;
    AVStream *stream = nullptr;
    auto nb_streams = pfc->nb_streams;
    for(auto i = 0U; i < nb_streams; ++i) {
        auto s = pfc->streams[i];
        if(s->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            stream = s;
            break;
        }
    }
    if(!stream) return;
    auto codec = avcodec_find_decoder(stream->codec->codec_id);
    if(!codec) return;
    auto pcc = avcodec_alloc_context3(codec);
    m_pCodecContext = pcc;
    if(!pcc || avcodec_open2(pcc, codec, nullptr) < 0) return;
    m_OpenFlag = true;
    m_Thread   = thread(bind(&AudioDecoder::Main, this));
}

AudioDecoder::~AudioDecoder() {
    m_OpenFlag = false;
    m_Thread.join();
    auto pcc = m_pCodecContext;
    if(pcc) {
        avcodec_close(pcc);
        av_free(pcc);
    }
    auto ppfc = &m_pFormatContext;
    avformat_close_input(ppfc);
}

bool AudioDecoder::IsOpen(void) {
    return m_OpenFlag;
}

void AudioDecoder::SeekSet(void) {
    m_SeekFlag = true;
    return;
}

size_t AudioDecoder::Read(float *data, size_t num) {
    m_Mutex.lock();
    for(size_t i = 0; i < num; i++) {
        if(m_Buffer.empty()) {
            num = i;
            break;
        }
        data[i] = m_Buffer.front();
        m_Buffer.pop();
    }
    m_Mutex.unlock();
    return num;
}

size_t AudioDecoder::GetNumChannels(void) {
    return m_pCodecContext->channels;
}

size_t AudioDecoder::GetSampleRate(void) {
    return m_pCodecContext->sample_rate;
}

void AudioDecoder::Main(void) {
    while(m_OpenFlag) {
//        avcodec_decode_audio4(m_pCodecContext, frame, &frameFinished, packet);
        usleep(1);
    }
    return;
}

}  // namespace WaveletAnalyzer
