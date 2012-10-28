//============================================================================
//
//  audio/device/portaudiowrapper.hpp
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

#ifndef _AUDIO_DEVICE_PORTAUDIOWRAPPER_HPP_
#define _AUDIO_DEVICE_PORTAUDIOWRAPPER_HPP_

#include <portaudio.h>
#include <stdexcept>
#include <string>
#include <mutex>
#include "../format.hpp"

namespace WaveletAnalyzer {

namespace Audio {

namespace Device {

using std::exception;
using std::string;
using std::call_once;
using std::once_flag;

class PortAudioWrapper {

    class paw_exception : public exception {

    public:
        paw_exception(const string &msg);

    public:
        const char *what(void) const throw();

    private:
        string m_Message;

    };

public:
    PortAudioWrapper();
    virtual ~PortAudioWrapper();

public:
    int         GetDeviceCount(void) const;
    int         GetDefaultInputDevice(void) const;
    int         GetDefaultOutputDevice(void) const;
    const char *GetDeviceName(int id) const;
    bool        IsInputDevice(int id) const;
    bool        IsOutputDevice(int id) const;

public:
    bool Open(int id, SampleFormat &sfmt, const Callback &func);
    void Close(void);

public:
    void Start(void) const;
    void Stop(void) const;

public:
    double GetTime(void) const;

private:
    static int Main(const void *input, void *output,
            unsigned long frameCount,
            const PaStreamCallbackTimeInfo *timeInfo,
            PaStreamCallbackFlags statusFlags, void *userData);

private:
    Callback  m_Function;
    PaStream *m_pStream;
    size_t    m_TimeNum;
    size_t    m_TimeDen;

private:
    static once_flag m_InitFlag;

};

}  // namespace Device

}  // namespace Audio

}  // namespace WaveletAnaryzer

#endif /* _AUDIO_DEVICE_PORTAUDIOWRAPPER_HPP_ */
