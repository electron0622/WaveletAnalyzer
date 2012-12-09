#include "wavelet.hpp"

Wavelet::Wavelet(){
    m_Table    = nullptr;
    m_Temp0    = nullptr;
    m_Temp1    = nullptr;
    m_Temp2    = nullptr;
    m_Temp3    = nullptr;
    m_TimeSize = 0;
    m_FreqSize = 0;
}

Wavelet::~Wavelet(){
    delete[] m_Table;
    delete[] m_Temp0;
    delete[] m_Temp1;
    delete[] m_Temp2;
    delete[] m_Temp3;
}

bool Wavelet::Init(size_t time_size, float time_min, float time_max,
                   size_t freq_size, float freq_min, float freq_max,
                   MotherWaveletFunc &mother_wavelet,
                   InterpolationFunc &interpolation)
{
    if(!mother_wavelet || !interpolation || !m_Fourier.Init(time_size)) return false;
    delete[] m_Temp2;
    delete[] m_Temp1;
    delete[] m_Table;
    auto table  = new complex<float>[time_size * freq_size];
    m_Table     = table;
    m_Temp0     = new complex<float>[time_size];
    m_Temp1     = new complex<float>[time_size];
    m_Temp2     = new complex<float>[time_size];
    m_Temp3     = new complex<float>[time_size];
    m_TimeSize  = time_size;
    m_FreqSize  = freq_size;
    int   half  = time_size >> 1;
    int   mask  = time_size -  1;
    float base  = (time_max - time_min) / time_size;
    float inv_f = 1.0f / freq_size;
    for(size_t f = 0; f < freq_size; f++){
        float scale = base * interpolation(freq_min, freq_max, f * inv_f);
        for(int t = -half; t < half; t++) m_Temp1[t & mask] = scale * mother_wavelet(t * scale);
        m_Fourier.FFT(table, m_Temp1);
        table += time_size;
    }
    return true;
}

void Wavelet::Exec(float *dst, const float *src){
    std::complex<float> *table = m_Table;
    for(size_t t = 0; t < m_TimeSize; t++) m_Temp0[t] = src[t];
    m_Fourier.FFT(m_Temp1, m_Temp0);
    for(size_t f = 0; f < m_FreqSize; f++){
        for(size_t t = 0; t < m_TimeSize; t++) m_Temp2[t] = table[t] * m_Temp1[t];
        m_Fourier.IFFT(m_Temp3, m_Temp2);
        for(size_t t = 0; t < m_TimeSize; t++) dst[t] = std::abs(m_Temp3[t]);
        table += m_TimeSize;
        dst   += m_TimeSize;
    }
    return;
}
