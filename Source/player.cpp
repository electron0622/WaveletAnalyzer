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

Player::Player() : m_pReader(nullptr), m_pWriter(nullptr),
        m_pThread(nullptr), m_PlayFlag(false), m_RecordFlag(false),
        m_StopFlag(false), m_EndFlag(false) {
}

Player::~Player() {
    if(m_pThread) {
        m_EndFlag = true;
        m_pThread->join();
    }
    delete m_pThread;
}

bool Player::Init(Util::IO *pReader, Util::IO *pWriter) {
    if(m_pThread || !pReader || !pWriter) return false;
    m_pReader = pReader;
    m_pWriter = pWriter;
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

#include <stdio.h>
void Player::Main(void) {
/*
    Audio::Device::Writer device;
    while(m_pReader->Read(nullptr, 0)<0x800) usleep(1);
    device.SetSampleRate(m_pReader->GetSampleRate());
    device.SetNumChannels(m_pReader->GetNumChannels());
    if(!device.Open(device.GetDefaultDeviceName())) return;
*/
    while(!m_EndFlag) {
//        printf("%10.3f\n", (double)device.Tell()/(m_pReader->GetSampleRate()*m_pReader->GetNumChannels()*sizeof(float)));
        if(m_StopFlag) {
            Reset();
            continue;
        }
        if(m_PlayFlag) {
            Update();
            continue;
        }
        usleep(1);
        float tmp[0x800];
        if(m_pWriter->Write(nullptr, 0) <= 0x800 * sizeof(float)) continue;
        m_pReader->Read(tmp, 0x800 * sizeof(float));
        m_pWriter->Write(tmp, 0x800 * sizeof(float));
    }
    m_pReader->Close();
    m_pWriter->Close();
    return;
}

void Player::Update(void) {
//    int data[4096];
//    m_pDecoder->Read(data, 4096);
    return;
}

void Player::Reset(void) {
    m_pReader->Seek(0);
    m_PlayFlag   = false;
    m_RecordFlag = false;
    m_StopFlag   = false;
    return;
}

}  // namespace WaveletAnalyzer
