#ifndef WAVELET_HPP
#define WAVELET_HPP

#include <complex>
#include <functional>
#include "fourier.hpp"

using std::complex;
using std::function;

typedef function<complex<float> (float)>      MotherWaveletFunc;
typedef function<float (float, float, float)> InterpolationFunc;

class Wavelet{

public:
    Wavelet();
    ~Wavelet();

public:
    bool Init(size_t time_size, float time_min, float time_max,
              size_t freq_size, float freq_min, float freq_max,
              MotherWaveletFunc &mother_wavelet,
              InterpolationFunc &interpolation);

public:
    void Exec(float *dst, const float *src);

private:
    Fourier         m_Fourier;
    complex<float> *m_Table;
    complex<float> *m_Temp0;
    complex<float> *m_Temp1;
    complex<float> *m_Temp2;
    complex<float> *m_Temp3;
    size_t          m_TimeSize;
    size_t          m_FreqSize;

};

#endif // WAVELET_HPP
