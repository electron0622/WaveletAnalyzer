//============================================================================
//
//  analyzer.hpp
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

#ifndef _ANALYZER_HPP_
#define _ANALYZER_HPP_

#include <memory>
#include <vector>
#include <deque>
#include <queue>
#include <thread>
#include <mutex>
#include "wavelet.hpp"

namespace WaveletAnalyzer {

using std::shared_ptr;
using std::vector;
using std::deque;
using std::queue;
using std::thread;
using std::mutex;

typedef shared_ptr<float> float_ptr;

class Analyzer {

public:
    Analyzer();
    ~Analyzer();

public:
    bool Init(float sigma, float time, size_t rate,
              float fmin,  float fmax, size_t fnum);

public:
    bool SetCacheSize(size_t size);

public:
    size_t GetCacheSize(void) const;
    const float_ptr *GetData(void);

public:
    size_t Write(const void *data, size_t size);

public:
    static complex<float> MotherWavelet(float x, float sigma);
    static float Interpolation(float min, float max, float alpha);

private:
    void Main(void);

private:
    Wavelet           m_Wavelet;
    vector<float_ptr> m_VecData;
    deque<float_ptr>  m_QueData;
    queue<float>      m_Buffer;
    thread            m_Thread;
    mutex             m_InitMutex;
    mutex             m_DataMutex;
    mutex             m_BufMutex;
    float            *m_pSrcTemp;
    float            *m_pDstTemp;
    float             m_Sigma;
    float             m_SampleTime;
    float             m_FreqMin;
    float             m_FreqMax;
    size_t            m_FreqNum;
    size_t            m_SampleRate;
    size_t            m_ClipNum;
    size_t            m_TimeNum;
    size_t            m_MaxDataNum;
    bool              m_EndFlag;

};

}  // namespace WaveletAnalyzer

#endif /* _ANALYZER_HPP_ */
