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

FFmpegWrapper::FFmpegWrapper() {
    auto init = [](void) {
        av_register_all();
        return;
    };
    call_once(m_InitFlag, init);
}

FFmpegWrapper::~FFmpegWrapper() {
    Close();
}

bool FFmpegWrapper::Open(const char *name,
        SampleFormat &sfmt, const Callback &func) {
    return true;
}

void FFmpegWrapper::Close(void) {
    return;
}

void FFmpegWrapper::Start(void) const {
    return;
}

void FFmpegWrapper::Stop(void) const {
    return;
}

double FFmpegWrapper::GetTime(void) const {
    return 0.0;
}

void FFmpegWrapper::Main(void) {
    return;
}

once_flag FFmpegWrapper::m_InitFlag;

}  // namespace File

}  // namespace Audio

}  // namespace WaveletAnaryzer
