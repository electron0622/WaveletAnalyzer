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

#include "player.hpp"

namespace WaveletAnalyzer {

Player::Player() : m_PlayFlag(false), m_StopFlag(false), m_EndFlag(false) {
    m_Thread = thread(bind(Main, this));
}

Player::~Player() {
    m_EndFlag = true;
    m_Thread.join();
}

void Player::Main(void) {
    while(!m_EndFlag) {
        if(m_PlayFlag) Update();
        if(m_StopFlag) Reset();
        Wait();
    }
    return;
}

void Player::Update(void) {
    return;
}

void Player::Reset(void) {
    return;
}

void Player::Wait(void) {
    sleep_for(milliseconds(1));
    return;
}

}  // namespace WaveletAnalyzer
