//============================================================================
//
//  audio/device/writer.hpp
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

#ifndef _AUDIO_DEVICE_WRITER_HPP_
#define _AUDIO_DEVICE_WRITER_HPP_

#include <string>
#include <map>
#include <vector>
#include <queue>
#include <mutex>
#include "portaudiowrapper.hpp"
#include "../../util/io.hpp"

namespace WaveletAnalyzer {

namespace Audio {

namespace Device {

using std::string;
using std::map;
using std::vector;
using std::queue;
using std::mutex;

class Writer : protected PortAudioWrapper, public Util::IO {

public:
    Writer();
    ~Writer();

public:
    bool SetCacheSize(size_t size);
    bool SetSampleRate(size_t rate);
    bool SetNumChannels(size_t ch);

public:
    size_t GetCacheSize(void) const;
    size_t GetSampleRate(void) const;
    size_t GetNumChannels(void) const;

public:
    int         GetDeviceCount(void) const;
    const char *GetDeviceName(int id) const;
    const char *GetDefaultDeviceName(void) const;
    double      GetTime(void) const;

public:
    bool   Open(const char *name);
    void   Close(void);
    size_t Write(const void *data, size_t size);
    size_t Tell(void);

private:
    void Main(const void *, void *output, size_t count);

private:
    map<string, int> m_DeviceMap;
    vector<int>      m_DeviceTable;
    queue<float>     m_Buffer;
    mutex            m_Mutex;
    SampleFormat     m_SampleFormat;
    size_t           m_MaxBufNum;
    size_t           m_WritePos;

};

}  // namespace Device

}  // namespace Audio

}  // namespace WaveletAnaryzer

#endif /* _AUDIO_DEVICE_WRITER_HPP_ */
