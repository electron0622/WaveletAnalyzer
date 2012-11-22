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
#include <vector>
#include "player.hpp"
#include "util/io.hpp"
#include "wavelet.hpp"

namespace WaveletAnalyzer {

using std::bind;
using std::vector;

Player::Player() : m_pThread(nullptr), m_DataNum(0x100), m_Volume(1.0f),
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
    auto pAnalyzer = io_ptr(new Util::IO);
    return Init(pInput, pOutput, pAnalyzer);
}

bool Player::Init(io_ptr &pInput, io_ptr &pOutput, io_ptr &pAnalyzer) {
    if(m_pThread || !pInput || !pOutput || !pAnalyzer) return false;
    m_pInput    = pInput;
    m_pOutput   = pOutput;
    m_pAnalyzer = pAnalyzer;
    m_pThread   = new thread(bind(&Player::Main, this));
    return true;
}

bool Player::SetDataSize(size_t size) {
    if(size < sizeof(float)) return false;
    m_DataNum = size / sizeof(float);
    return true;
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
    vector<float> data(m_DataNum);
    while(!m_EndFlag) {
        if(m_StopFlag) {
            m_pInput->Seek(0);
            m_PlayFlag = false;
            m_StopFlag = false;
        }
        if(!m_PlayFlag) {
            usleep(1);
            continue;
        }
        auto num  = m_DataNum;
        auto size = num * sizeof(float);
        if(m_pOutput->Write(nullptr, 0) <= size) {
            usleep(1);
            continue;
        }
        if(m_pInput ->Read (nullptr, 0) <= size) {
            usleep(1);
            continue;
        }
        data.resize(num);
        m_pInput   ->Read (&data[0], size);
        auto vol = m_Volume;
        for(size_t i = 0; i < num; i++) data[i] *= vol;
        m_pOutput  ->Write(&data[0], size);
        m_pAnalyzer->Write(&data[0], size);
    }
    m_pInput   ->Close();
    m_pOutput  ->Close();
    m_pAnalyzer->Close();
    return;
}

}  // namespace WaveletAnalyzer
