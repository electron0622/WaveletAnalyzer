//============================================================================
//
//  audio/file/reader.cpp
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

#include <functional>
#include "reader.hpp"
#include <unistd.h>

namespace WaveletAnalyzer {

namespace Audio {

namespace File {

using std::bind;

Reader::Reader() : m_SampleFormat({0, 0, BF_UNKNOWN}),
        m_MaxBufNum(0x4000), m_ReadPos(0), m_EndFlag(false) {
}

Reader::~Reader() {
    Close();
}

bool Reader::SetCacheSize(size_t size) {
    if(size < sizeof(float)) return false;
    m_MaxBufNum = size / sizeof(float);
    return true;
}

bool Reader::SetSampleRate(size_t rate) {
    m_SampleFormat.SampleRate = rate;
    return true;
}

bool Reader::SetNumChannels(size_t ch) {
    m_SampleFormat.NumChannels = ch;
    return true;
}

size_t Reader::GetCacheSize(void) const {
    return m_MaxBufNum * sizeof(float);
}

size_t Reader::GetSampleRate(void) const {
    return m_SampleFormat.SampleRate;
}

size_t Reader::GetNumChannels(void) const {
    return m_SampleFormat.NumChannels;
}

bool Reader::Open(const char *name) {
    if(m_Thread.joinable()) return false;
    if(!FFmpegWrapper::Open(name, m_SampleFormat)) return false;
    m_EndFlag = false;
    m_Thread  = thread(bind(&Reader::Main, this));
    return true;
}

void Reader::Close(void) {
    if(m_Thread.joinable()) {
        m_EndFlag = true;
        m_Thread.join();
        FFmpegWrapper::Close();
        queue<float>().swap(m_Buffer);
        m_ReadPos = 0;
    }
    return;
}

size_t Reader::Read(void *data, size_t size) {
    m_Mutex.lock();
    if(data) {
        size_t num = size / sizeof(float);
        for(size_t i = 0; i < num; i++) {
            if(m_Buffer.empty()) {
                size = i * sizeof(float);
                break;
            }
            ((float *)data)[i] = m_Buffer.front();
            m_Buffer.pop();
            m_ReadPos += sizeof(float);
        }
    } else {
        size = m_Buffer.size() * sizeof(float);
    }
    m_Mutex.unlock();
    return size;
}

bool Reader::Seek(size_t offset) {
    offset /= sizeof(float);
    size_t scale;
    switch(m_SampleFormat.FormatId) {
    case BF_S8:  scale = sizeof(int8_t);   break;
    case BF_U8:  scale = sizeof(uint8_t);  break;
    case BF_S16: scale = sizeof(int16_t);  break;
    case BF_U16: scale = sizeof(uint16_t); break;
    case BF_S32: scale = sizeof(int32_t);  break;
    case BF_U32: scale = sizeof(uint32_t); break;
    case BF_F32: scale = sizeof(float);    break;
    case BF_F64: scale = sizeof(double);   break;
    default:     scale = 0;                break;
    }
    m_Mutex.lock();
    auto successed = FFmpegWrapper::Seek(offset * scale);
    if(successed) {
        queue<float>().swap(m_Buffer);
        m_ReadPos = offset * sizeof(float);
    }
    m_Mutex.unlock();
    return successed;
}

size_t Reader::Tell(void) {
    return m_ReadPos;
}

void Reader::Main(void) {
    switch(m_SampleFormat.FormatId) {
    case BF_S8:  FillBuffer<int8_t  >(0x00000000, 1.0f / 0x00000080); break;
    case BF_U8:  FillBuffer<uint8_t >(0x00000080, 1.0f / 0x00000080); break;
    case BF_S16: FillBuffer<int16_t >(0x00000000, 1.0f / 0x00008000); break;
    case BF_U16: FillBuffer<uint16_t>(0x00008000, 1.0f / 0x00008000); break;
    case BF_S32: FillBuffer<int32_t >(0x00000000, 1.0f / 0x80000000); break;
    case BF_U32: FillBuffer<uint32_t>(0x80000000, 1.0f / 0x80000000); break;
    case BF_F32: FillBuffer<float   >(0x00000000, 1.0f / 0x00000001); break;
    case BF_F64: FillBuffer<double  >(0x00000000, 1.0f / 0x00000001); break;
    default:                                                          break;
    }
    return;
}

template<class T>
void Reader::FillBuffer(T bias, float scale) {
    T data[0x1000];
    m_Mutex.lock();
    auto num = FFmpegWrapper::Read(data, 0x1000 * sizeof(T)) / sizeof(T);
    m_Mutex.unlock();
    m_SampleFormat.SampleRate  = FFmpegWrapper::GetSampleRate();
    m_SampleFormat.NumChannels = FFmpegWrapper::GetNumChannels();
    while(!m_EndFlag && num > 0) {
        m_Mutex.lock();
        auto full = m_Buffer.size() >= m_MaxBufNum;
        m_Mutex.unlock();
        if(full) {
            usleep(1);
            continue;
        }
        m_Mutex.lock();
        for(size_t i = 0; i < num; i++) {
            m_Buffer.push((data[i] - bias) * scale);
        }
        num = FFmpegWrapper::Read(data, 0x1000 * sizeof(T)) / sizeof(T);
        m_Mutex.unlock();
    }
    return;
}

}  // namespace File

}  // namespace Audio

}  // namespace WaveletAnaryzer
