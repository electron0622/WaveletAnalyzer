//============================================================================
//
//  audio/device/writer.cpp
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
#include "writer.hpp"

namespace WaveletAnalyzer {

namespace Audio {

namespace Device {

using std::bind;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

Writer::Writer() : m_SampleFormat({0, 0, BF_F32}),
        m_MaxBufNum(0x4000), m_WritePos(0) {
    int num = PortAudioWrapper::GetDeviceCount();
    for(int i = 0; i < num; i++) if(PortAudioWrapper::IsOutputDevice(i)){
        m_DeviceMap[PortAudioWrapper::GetDeviceName(i)] = i + 1;
        m_DeviceTable.push_back(i);
    }
}

Writer::~Writer() {
    Close();
}

void Writer::SetCacheSize(size_t size) {
    m_MaxBufNum = (size + (sizeof(float) - 1)) / sizeof(float);
    return;
}

void Writer::SetSampleRate(size_t rate) {
    m_SampleFormat.SampleRate = rate;
    return;
}

void Writer::SetNumChannels(size_t ch) {
    m_SampleFormat.NumChannels = ch;
    return;
}

size_t Writer::GetCacheSize(void) const {
    return m_MaxBufNum * sizeof(float);
}

size_t Writer::GetSampleRate(void) const {
    return m_SampleFormat.SampleRate;
}

size_t Writer::GetNumChannels(void) const {
    return m_SampleFormat.NumChannels;
}

int Writer::GetDeviceCount(void) const {
    return (int)m_DeviceTable.size();
}

const char *Writer::GetDeviceName(int id) const {
    if(id >= PortAudioWrapper::GetDeviceCount()) return nullptr;
    return PortAudioWrapper::GetDeviceName(m_DeviceTable[id]);
}

const char *Writer::GetDefaultDeviceName(void) const {
    auto id = PortAudioWrapper::GetDefaultOutputDevice();
    return PortAudioWrapper::GetDeviceName(id);
}

double Writer::GetTime(void) const {
    return PortAudioWrapper::GetTime();
}

bool Writer::Open(const char *name) {
    int id = m_DeviceMap[name] - 1;
    if(id < 0) return false;
    SampleFormat &sfmt = m_SampleFormat;
    Callback      func = bind(&Writer::Main, this, _1, _2, _3);
    m_WritePos         = 0;
    if(!PortAudioWrapper::Open(id, sfmt, func)) return false;
    PortAudioWrapper::Start();
    return true;
}

void Writer::Close(void) {
    PortAudioWrapper::Stop();
    PortAudioWrapper::Close();
    queue<float>().swap(m_Buffer);
    m_WritePos = 0;
    return;
}

size_t Writer::Write(const void *data, size_t size) {
    m_Mutex.lock();
    if(data) {
        size_t num = size / sizeof(float);
        for(size_t i = 0; i < num; i++) {
            if(m_Buffer.size() >= m_MaxBufNum) {
                size = i * sizeof(float);
                break;
            }
            m_Buffer.push(((const float *)data)[i]);
        }
    } else {
        size = (m_MaxBufNum - m_Buffer.size()) * sizeof(float);
    }
    m_Mutex.unlock();
    return size;
}

size_t Writer::Tell(void) {
    return m_WritePos;
}

void Writer::Main(const void *, void *output, size_t count) {
    size_t num = count * m_SampleFormat.NumChannels;
    memset(output, 0, num * sizeof(float));
    m_Mutex.lock();
    for(size_t i = 0; i < num; i++) {
        if(m_Buffer.empty()) break;
        ((float *)output)[i] = m_Buffer.front();
        m_Buffer.pop();
        m_WritePos += sizeof(float);
    }
    m_Mutex.unlock();
    return;
}

}  // namespace Device

}  // namespace Audio

}  // namespace WaveletAnaryzer
