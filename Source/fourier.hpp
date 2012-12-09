#ifndef FOURIER_HPP
#define FOURIER_HPP

#include <complex>

class Fourier{

public:
    Fourier();
    ~Fourier();

public:
    bool Init(size_t size);

public:
    void FFT(std::complex<float> *dst, const std::complex<float> *src);
    void IFFT(std::complex<float> *dst, const std::complex<float> *src);

private:
    std::complex<float> *m_Table;
    size_t               m_Size;
    size_t               m_Mask;
    size_t               m_Bits;

};

#endif // FOURIER_HPP
