//============================================================================
//
//  player.cpp
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

#include <unistd.h>
#include <functional>
#include "player.hpp"
#include "util/io.hpp"

namespace WaveletAnalyzer {

using std::bind;

Player::Player() : m_pThread(nullptr), m_Volume(1.0f),
        m_PlayFlag(false), m_StopFlag(false), m_EndFlag(false) {
}

Player::~Player() {
    if(m_pThread) {
        m_EndFlag = true;
        m_pThread->join();
    }
    delete m_pThread;
}

bool Player::Init(io_ptr &pInput, io_ptr &pOutput) {
    if(m_pThread || !pInput || !pOutput) return false;
    m_pInput  = pInput;
    m_pOutput = pOutput;
    m_pThread = new thread(bind(&Player::Main, this));
    return true;
}

const char *Player::GetMainGraph(size_t width, size_t height) {
    return nullptr;
}

const char *Player::GetSubGraph(size_t width, size_t height) {
    return nullptr;
}

bool Player::SetVolume(float vol) {
    if(vol < 0.0f) return false;
    m_Volume = vol;
    return true;
}

void Player::Play(void) {
    m_PlayFlag = true;
    return;
}

void Player::Pause(void) {
    m_PlayFlag = false;
    return;
}

void Player::Stop(void) {
    m_StopFlag = true;
    return;
}

void Player::Main(void) {
    constexpr size_t num  = 0x1000;
    constexpr size_t size = num * sizeof(float);
    float data[num];
    Wavelet w;
    float *dst = new float[num * 1024];
    MotherWaveletFunc mfunc = [](float x) {
        constexpr float one_over_sqrt_two_pi = 1.0 / std::sqrt(2.0 * M_PI);
        return one_over_sqrt_two_pi * std::exp(complex<float>(x * x * (-1.0f / 2.0f), (float)(2.0f * M_PI) * x));
    };
    InterpolationFunc ifunc = [](float min, float max, float alpha) {
        return min * (1 - alpha) + max * alpha;
    };
    w.Init(0.0f, num/44100.0f, num, 20.0f, 20000.0f, 1024, mfunc, ifunc);
    while(!m_EndFlag) {
        if(m_StopFlag) {
            m_pInput->Seek(0);
            m_PlayFlag = false;
            m_StopFlag = false;
            continue;
        }
        usleep(1);
        if(m_pOutput->Write(nullptr, 0) <= size) continue;
        if(m_PlayFlag) {
            if(m_pInput->Read(nullptr, 0) <= size) continue;
            m_pInput->Read(data, size);
            auto vol = m_Volume;
            for(size_t i = 0; i < num; i++) data[i] *= vol;
            w.Exec(dst, data);
        } else {
            memset(data, 0, size);
        }
        m_pOutput->Write(data, size);
    }
    m_pInput ->Close();
    m_pOutput->Close();
    return;
}

}  // namespace WaveletAnalyzer
