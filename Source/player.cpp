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

#include <portaudio.h>
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

int callback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData ) {
    AudioReader *pReader = (AudioReader *)userData;
    pReader->Read((float *)outputBuffer, framesPerBuffer<<1);
    return 0;
}

void Player::Main(void) {
    PaError err;
    err = Pa_Initialize();
    if(err != paNoError) {
        Pa_Terminate();
        return;
    }
    while(m_pReader->Read(nullptr, 0)<0x1000) usleep(1);
    PaStream *stream;
    err = Pa_OpenDefaultStream(&stream, 0, m_pReader->GetNumChannels(), paFloat32, m_pReader->GetSampleRate(), 512, callback, m_pReader);
    if(err != paNoError) {
        Pa_Terminate();
        return;
    }
    err = Pa_StartStream(stream);
    if(err != paNoError) {
        Pa_Terminate();
        return;
    }
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
    Pa_StopStream(stream);
    Pa_Terminate();
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
