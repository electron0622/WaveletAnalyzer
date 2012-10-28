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
#include "audioreader.hpp"
#include "audiodecoder.hpp"

namespace WaveletAnalyzer {

using std::shared_ptr;
using std::make_shared;
using std::thread;

typedef shared_ptr<AudioReader> AudioReaderPtr;

inline AudioReaderPtr MakeAudioReader(void) {
    return make_shared<AudioReader>();
}

inline AudioReaderPtr MakeAudioDecoder(const char *path, int cache) {
    return make_shared<AudioDecoder>(path, cache);
}

class Player {

public:
    Player();
    ~Player();

public:
    bool Init(const AudioReaderPtr &pReader);

public:
    void Play(void);
    void Pause(void);
    void Record(void);
    void Stop(void);

private:
    void Func(const void *in, void *out, size_t count);
    void Main(void);
    void Update(void);
    void Reset(void);

private:
    AudioReaderPtr m_pReader;
    thread*        m_pThread;
    bool           m_PlayFlag;
    bool           m_RecordFlag;
    bool           m_StopFlag;
    bool           m_EndFlag;

};

}  // namespace WaveletAnalyzer

#endif /* _PLAYER_HPP_ */
