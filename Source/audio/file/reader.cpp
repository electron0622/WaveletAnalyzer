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

#include "reader.hpp"

namespace WaveletAnalyzer {

namespace Audio {

namespace File {

Reader::Reader() : m_SampleFormat({0, 0, BF_VOID}),
        m_MaxBufNum(0x4000), m_ReadPos(0) {
}

Reader::~Reader() {
}

void Reader::SetCacheSize(size_t size) {
    m_MaxBufNum = (size + (sizeof(float) - 1)) / sizeof(float);
    return;
}

void Reader::SetSampleRate(size_t rate) {
    m_SampleFormat.SampleRate = rate;
    return;
}

void Reader::SetNumChannels(size_t ch) {
    m_SampleFormat.NumChannels = ch;
    return;
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
    FFmpegWrapper::Close();
    if(!FFmpegWrapper::Open(name, m_SampleFormat)) return false;
    return true;
}

void Reader::Close(void) {
    return;
}

size_t Reader::Read(void *data, size_t size) {
    return 0;
}

bool Reader::Seek(size_t offset) {
    return false;
}

size_t Reader::Tell(void) {
    return m_ReadPos;
}

void Reader::Main(void) {
    return;
}

}  // namespace File

}  // namespace Audio

}  // namespace WaveletAnaryzer
