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

#include <memory>
#include <thread>
#include "wavelet.hpp"
#include "util/io.hpp"

namespace WaveletAnalyzer {

using std::shared_ptr;
using std::thread;

typedef shared_ptr<Util::IO> io_ptr;

class Player {

public:
    Player();
    ~Player();

public:
    bool Init(io_ptr &pInput, io_ptr &pOutput);

public:
    const char *GetMainGraph(size_t width, size_t height);
    const char *GetSubGraph(size_t width, size_t height);

public:
    bool SetVolume(float vol);

public:
    void Play(void);
    void Pause(void);
    void Stop(void);

private:
    void Main(void);

private:
    io_ptr  m_pInput;
    io_ptr  m_pOutput;
    thread *m_pThread;
    float   m_Volume;
    bool    m_PlayFlag;
    bool    m_StopFlag;
    bool    m_EndFlag;

};

}  // namespace WaveletAnalyzer

#endif /* _PLAYER_HPP_ */
