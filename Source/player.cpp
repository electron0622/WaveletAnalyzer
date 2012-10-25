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
#include "audiodecoder.hpp"
#include "player.hpp"

namespace WaveletAnalyzer {

using std::bind;

Player::Player() : m_pReader(nullptr), m_pThread(nullptr),
        m_PlayFlag(false), m_RecordFlag(false),
        m_StopFlag(false), m_EndFlag(false) {
}

Player::~Player() {
    if(m_pThread) {
        m_EndFlag = true;
        m_pThread->join();
    }
    delete m_pThread;
    delete m_pReader;
}

bool Player::Init(const char *path) {
    if(m_pThread) return false;
    m_pReader = new AudioDecoder(path, 0x10000);
    if(!m_pReader->IsOpen()) {
        delete m_pReader;
        m_pReader = nullptr;
        return false;
    }
    usleep(1000);
    auto ch = m_pReader->GetNumChannels();
    auto sr = m_pReader->GetSampleRate();
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

void Player::Record(void) {
    m_RecordFlag = true;
    return;
}

void Player::Stop(void) {
    m_StopFlag = true;
    return;
}

void Player::Main(void) {
    while(!m_EndFlag) {
        if(m_StopFlag) {
            Reset();
            continue;
        }
        if(m_PlayFlag) {
            Update();
            continue;
        }
        usleep(1);
    }
    return;
}

void Player::Update(void) {
//    int data[4096];
//    m_pDecoder->Read(data, 4096);
    return;
}

void Player::Reset(void) {
    m_pReader->SeekSet();
    m_PlayFlag   = false;
    m_RecordFlag = false;
    m_StopFlag   = false;
    return;
}

}  // namespace WaveletAnalyzer
