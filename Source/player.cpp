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

Player::Player() : m_pThread(nullptr), m_PlayFlag(false),
        m_StopFlag(false), m_EndFlag(false) {
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

const char *Player::GetMainGraph(size_t width, size_t height) {
    return nullptr;
}

const char *Player::GetSubGraph(size_t width, size_t height) {
    return nullptr;
}

void Player::Main(void) {
    constexpr size_t size = 0x1000;
    float data[size / sizeof(float)];
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
