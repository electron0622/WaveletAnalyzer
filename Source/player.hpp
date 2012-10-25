//============================================================================
//
//  player.hpp
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

#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <thread>

namespace WaveletAnalyzer {

using std::thread;

class AudioReader;

class Player {

public:
    Player();
    ~Player();

public:
    bool Init(const char *path);

public:
    void Play(void);
    void Pause(void);
    void Record(void);
    void Stop(void);

private:
    void Main(void);
    void Update(void);
    void Reset(void);

private:
    AudioReader *m_pReader;
    thread      *m_pThread;
    bool         m_PlayFlag;
    bool         m_RecordFlag;
    bool         m_StopFlag;
    bool         m_EndFlag;

};

}  // namespace WaveletAnalyzer

#endif /* _PLAYER_HPP_ */
