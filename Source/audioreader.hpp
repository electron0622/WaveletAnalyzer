//============================================================================
//
//  audioreader.hpp
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

#ifndef _AUDIOREADER_HPP_
#define _AUDIOREADER_HPP_

namespace WaveletAnalyzer {

class AudioReader {

public:
    AudioReader();
    virtual ~AudioReader();

public:
    virtual bool   IsOpen(void);
    virtual void   SeekSet(void);
    virtual size_t Read(float *data, size_t num);
    virtual size_t GetNumChannels(void);
    virtual size_t GetSampleRate(void);

};

}  // namespace WaveletAnaryzer

#endif /* _AUDIOREADER_HPP_ */
