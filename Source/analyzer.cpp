//============================================================================
//
//  analyzer.cpp
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
#include <algorithm>
#include "analyzer.hpp"

namespace WaveletAnalyzer {

using std::copy;
using std::bind;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::default_delete;

static size_t flp2(size_t x) {
    --x;
    x |= x >> 0x01;
    x |= x >> 0x02;
    x |= x >> 0x04;
    x |= x >> 0x08;
    x |= x >> 0x10;
    return ++x;
}

Analyzer::Analyzer() : m_pSrcTemp(nullptr), m_pDstTemp(nullptr),
        m_Sigma(0.0f), m_SampleTime(0.0f), m_FreqMin(0.0f),
        m_FreqMax(0.0f), m_FreqNum(0), m_SampleRate(0),
        m_ClipNum(0), m_TimeNum(0), m_MaxDataNum(0), m_EndFlag(false) {
}

Analyzer::~Analyzer() {
    if(m_Thread.joinable()) {
        m_EndFlag = true;
        m_Thread.join();
    }
    delete[] m_pSrcTemp;
    delete[] m_pDstTemp;
}

bool Analyzer::Init(float sigma, float time, size_t rate,
                    float fmin,  float fmax, size_t fnum) {
    if(sigma == m_Sigma && rate == m_SampleRate &&
            fmin == m_FreqMin && fmax == m_FreqMax && fnum == m_FreqNum) {
        size_t dnum  = time * rate;
        m_SampleTime = time;
        m_MaxDataNum = dnum;
        while(m_QueData.size() < dnum) {
            auto ptr = new float[fnum];
            m_QueData.push_front(float_ptr(ptr, default_delete<float[]>()));
        }
        return true;
    }
    m_InitMutex.lock();
    m_BufMutex.lock();
    m_DataMutex.lock();
    if(rate != m_SampleRate) {
        queue<float>().swap(m_Buffer);
    }
    if(fmin != m_FreqMin || fmax != m_FreqMax || fnum != m_FreqNum) {
        m_QueData.clear();
    }
    size_t dnum  = time * rate;
    m_Sigma      = sigma;
    m_SampleTime = time;
    m_SampleRate = rate;
    m_FreqNum    = fnum;
    m_FreqMin    = fmin;
    m_FreqMax    = fmax;
    m_MaxDataNum = dnum;
    size_t clip  = 3.0f * sigma * rate / fmin;
    size_t tnum  = flp2(clip << 2);
    float  tmin  = -((float)tnum) / rate;
    float  tmax  = 0.0f;
    m_ClipNum    = clip;
    m_TimeNum    = tnum;
    delete[] m_pSrcTemp;
    delete[] m_pDstTemp;
    m_pSrcTemp = new float[tnum];
    m_pDstTemp = new float[tnum * fnum];
    while(m_QueData.size() < dnum) {
        auto ptr = new float[fnum];
        m_QueData.push_front(float_ptr(ptr, default_delete<float[]>()));
    }
    MotherWaveletFunc mfunc = bind(&Analyzer::MotherWavelet, _1, sigma);
    InterpolationFunc ifunc = bind(&Analyzer::Interpolation, _1, _2, _3);
    auto ret = m_Wavelet.Init(tnum,tmin,tmax,fnum,fmin,fmax,mfunc,ifunc);
    m_DataMutex.unlock();
    m_BufMutex.unlock();
    m_InitMutex.unlock();
    if(!m_Thread.joinable()) m_Thread = thread(bind(&Analyzer::Main, this));
    return ret;
}

const float_ptr *Analyzer::GetData(void) {
    m_DataMutex.lock();
    auto flag = m_QueData.size() < m_MaxDataNum;
    m_DataMutex.unlock();
    if(flag) return nullptr;
    m_DataMutex.lock();
    m_VecData.assign(m_QueData.begin(), m_QueData.end());
    m_DataMutex.unlock();
    return &m_VecData[0];
}

size_t Analyzer::Write(const void *data, size_t size) {
    m_BufMutex.lock();
    if(data) {
        size_t num = size / sizeof(float);
        for(size_t i = 0; i < num; i++) {
/*            if(m_Buffer.size() >= m_TimeNum) {
                size = i * sizeof(float);
                break;
            }
*/            m_Buffer.push(((const float *)data)[i]);
        }
    } else {
        size = (m_TimeNum - m_Buffer.size()) * sizeof(float);
    }
    m_BufMutex.unlock();
    return size;
}

complex<float> Analyzer::MotherWavelet(float x, float sigma) {
    constexpr float two_pi    = 2.0f * M_PI;
    constexpr float rsqrt_2pi = 1.0f / std::sqrt(two_pi);
    auto xx2ss = (x * x) / (2.0f * sigma * sigma);
    return (rsqrt_2pi / sigma) * std::exp(complex<float>(-xx2ss, two_pi * x));
}

float Analyzer::Interpolation(float min, float max, float alpha) {
    auto ln_min = std::log(min);
    auto ln_max = std::log(max);
    return std::exp((ln_max - ln_min) * alpha + ln_min);
}

void Analyzer::Main(void) {
    while(!m_EndFlag) {
        m_InitMutex.lock();
        auto src   = m_pSrcTemp;
        auto dst   = m_pDstTemp;
        auto clip  = m_ClipNum;
        auto num   = m_TimeNum;
        auto fnum  = m_FreqNum;
        auto dnum  = m_MaxDataNum;
        auto begin = clip << 1;
        auto end   = num - begin;
        m_BufMutex.lock();
        auto flag = m_Buffer.size() >= end;
        m_BufMutex.unlock();
        if(flag) {
            for(size_t i = 0, j = end; i < begin; i++, j++) src[i] = src[j];
            m_BufMutex.lock();
            for(size_t i = begin; i < num; i++) {
                src[i] = m_Buffer.front();
                m_Buffer.pop();
            }
            m_BufMutex.unlock();
            m_Wavelet.Exec(dst, src);
            m_DataMutex.lock();
            begin = clip;
            end   = num - clip;
            for(size_t i = begin; i < end; i++) {
                auto ptr = new float[fnum];
                auto tmp = float_ptr(ptr, default_delete<float[]>());
                for(size_t j = 0; j < fnum; j++) ptr[j] = dst[i + j * num];
                if(m_QueData.size() >= dnum) m_QueData.pop_front();
                m_QueData.push_back(tmp);
            }
            m_DataMutex.unlock();
        }
        m_InitMutex.unlock();
        if(!flag) usleep(1);
    }
    return;
}

}  // namespace WaveletAnalyzer
