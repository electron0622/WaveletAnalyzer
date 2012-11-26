//============================================================================
//
//  analyzer.cpp
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

#include "analyzer.hpp"

namespace WaveletAnalyzer {

Analyzer::Analyzer() {
}

Analyzer::~Analyzer() {
}

bool Analyzer::Open(const char *name) {
    return true;
}

void Analyzer::Close(void) {
    return;
}

size_t Analyzer::Write(const void *data, size_t size) {
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

void Analyzer::Main(void) {
    return;
}

}  // namespace WaveletAnalyzer
