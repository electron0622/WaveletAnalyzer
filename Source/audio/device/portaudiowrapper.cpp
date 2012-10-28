//============================================================================
//
//  audio/device/portaudiowrapper.cpp
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

#include <cstdlib>
#include "portaudiowrapper.hpp"

namespace WaveletAnalyzer {

namespace Audio {

namespace Device {

using std::atexit;

PortAudioWrapper::paw_exception::paw_exception(const string &msg) {
    m_Message = msg;
}

const char *PortAudioWrapper::paw_exception::what(void) const throw() {
    return m_Message.c_str();
}

PortAudioWrapper::PortAudioWrapper() : m_pStream(nullptr),
        m_TimeNum(0), m_TimeDen(1) {
    auto init = [](void) {
        auto error = Pa_Initialize();
        if(error != paNoError) {
            Pa_Terminate();
            throw paw_exception(Pa_GetErrorText(error));
        }
        atexit((void (*)(void))Pa_Terminate);
        return;
    };
    call_once(m_InitFlag, init);
}

PortAudioWrapper::~PortAudioWrapper() {
    Close();
}

int PortAudioWrapper::GetDeviceCount(void) const {
    return (int)Pa_GetDeviceCount();
}

int PortAudioWrapper::GetDefaultInputDevice(void) const {
    return (int)Pa_GetDefaultInputDevice();
}

int PortAudioWrapper::GetDefaultOutputDevice(void) const {
    return (int)Pa_GetDefaultOutputDevice();
}

const char *PortAudioWrapper::GetDeviceName(int id) const {
    return Pa_GetDeviceInfo((PaDeviceIndex)id)->name;
}

bool PortAudioWrapper::IsInputDevice(int id) const {
    return Pa_GetDeviceInfo((PaDeviceIndex)id)->maxInputChannels > 0;
}

bool PortAudioWrapper::IsOutputDevice(int id) const {
    return Pa_GetDeviceInfo((PaDeviceIndex)id)->maxOutputChannels > 0;
}

bool PortAudioWrapper::Open(int id,
        SampleFormat &sfmt, const Callback &func) {
    if(m_pStream) return false;
    PaSampleFormat format;
    switch(sfmt.FormatId) {
    case BF_S8:
        format = paInt8;
        break;
    case BF_U8:
        format = paUInt8;
        break;
    case BF_S16:
        format = paInt16;
        break;
    case BF_S32:
        format = paInt32;
        break;
    case BF_F32:
        format = paFloat32;
        break;
    default:
        return false;
    }
    PaStreamParameters input, output;
    PaStreamParameters *pinput  = nullptr;
    PaStreamParameters *poutput = nullptr;
    auto info = Pa_GetDeviceInfo((PaDeviceIndex)id);
    if(sfmt.NumChannels == 0) {
        size_t in  = (size_t)(-1);
        size_t out = (size_t)(-1);
        if(info->maxInputChannels  > 0) in  = info->maxInputChannels;
        if(info->maxOutputChannels > 0) out = info->maxOutputChannels;
        size_t ch = (in < out) ? in : out;
        sfmt.NumChannels = (ch != (size_t)(-1)) ? ch : 0;
    }
    if(sfmt.SampleRate == 0) {
        sfmt.SampleRate = (size_t)info->defaultSampleRate;
    }
    if(info->maxInputChannels >= (int)sfmt.NumChannels) {
        input.device                    = (PaDeviceIndex)id;
        input.channelCount              = (int)sfmt.NumChannels;
        input.sampleFormat              = format;
        input.suggestedLatency          = info->defaultLowInputLatency;
        input.hostApiSpecificStreamInfo = nullptr;
        pinput                          = &input;
    }
    if(info->maxOutputChannels >= (int)sfmt.NumChannels) {
        output.device                    = (PaDeviceIndex)id;
        output.channelCount              = (int)sfmt.NumChannels;
        output.sampleFormat              = format;
        output.suggestedLatency          = info->defaultLowOutputLatency;
        output.hostApiSpecificStreamInfo = nullptr;
        poutput                          = &output;
    }
    auto error = Pa_OpenStream(&m_pStream, pinput, poutput,
            (double)sfmt.SampleRate, 0x100, paNoFlag, Main, this);
    if(error != paNoError) return false;
    m_Function = func;
    m_TimeNum  = 0;
    m_TimeDen  = sfmt.SampleRate;
    return true;
}

void PortAudioWrapper::Close(void) {
    if(m_pStream) {
        Pa_CloseStream(m_pStream);
        m_pStream = nullptr;
    }
    m_TimeNum = 0;
    m_TimeDen = 1;
    return;
}

void PortAudioWrapper::Start(void) const {
    if(!m_pStream) return;
    Pa_StartStream(m_pStream);
    return;
}

void PortAudioWrapper::Stop(void) const {
    if(!m_pStream) return;
    Pa_StopStream(m_pStream);
    return;
}

double PortAudioWrapper::GetTime(void) const {
    return ((double)m_TimeNum) / ((double)m_TimeDen);
}

int PortAudioWrapper::Main(const void *input, void *output,
            unsigned long frameCount,
            const PaStreamCallbackTimeInfo *timeInfo,
            PaStreamCallbackFlags statusFlags, void *userData) {
    auto this_ptr = (PortAudioWrapper *)userData;
    this_ptr->m_TimeNum += (size_t)frameCount;
    this_ptr->m_Function(input, output, (size_t)frameCount);
    return 0;
}

once_flag PortAudioWrapper::m_InitFlag;

}  // namespace Device

}  // namespace Audio

}  // namespace WaveletAnalyzer
