#include "fourier.hpp"

static inline size_t pop32(size_t x){
    x = (x & 0x55555555) + ((x >>  1) & 0x55555555);
    x = (x & 0x33333333) + ((x >>  2) & 0x33333333);
    x = (x & 0x0f0f0f0f) + ((x >>  4) & 0x0f0f0f0f);
    x = (x & 0x00ff00ff) + ((x >>  8) & 0x00ff00ff);
    x = (x & 0x0000ffff) + ((x >> 16) & 0x0000ffff);
    return x;
}

static inline size_t rev32(size_t x){
    x = ((x & 0x55555555) <<  1) | ((x >>  1) & 0x55555555);
    x = ((x & 0x33333333) <<  2) | ((x >>  2) & 0x33333333);
    x = ((x & 0x0f0f0f0f) <<  4) | ((x >>  4) & 0x0f0f0f0f);
    x = ((x & 0x00ff00ff) <<  8) | ((x >>  8) & 0x00ff00ff);
    x = ((x & 0x0000ffff) << 16) | ((x >> 16) & 0x0000ffff);
    return x;
}

Fourier::Fourier(){
    m_Table   = nullptr;
    m_Size    = 0;
    m_Mask    = 0;
    m_Bits    = 0;
}

Fourier::~Fourier(){
    delete[] m_Table;
}

bool Fourier::Init(size_t size){
    if(size < 2 || size & (size - 1)) return false;
    delete[] m_Table;
    m_Size      = size;
    m_Table     = new std::complex<float>[size >>= 1];
    m_Mask      = size - 1;
    m_Bits      = pop32(m_Mask);
    float omega = 3.14159265358979323846 / size;
    #pragma omp parallel for
    for(size_t t = 0; t < size; t++){
        float theta = omega * t;
        m_Table[t]  = std::complex<float>(std::cos(theta), std::sin(theta));
    }
    return true;
}

void Fourier::FFT(std::complex<float> *dst, const std::complex<float> *src){
    std::complex<float> *table = m_Table;
    size_t               mask  = m_Mask;
    size_t               size  = m_Size;
    size_t               half  = size >> 1;
    int                  shl   = m_Bits;
    int                  shr   = 31 - shl;
    #pragma omp parallel for
    for(size_t i = 0; i < size; i++) dst[i] = src[rev32(i) >> shr]; // ビット逆転アドレッシング
    for(size_t bit = 1; shl >= 0; bit <<= 1, shl--){
        size_t x = bit - 1;
        size_t y = ~x;
        #pragma omp parallel for
        for(size_t i = 0; i < half; i++){
            size_t k = ((i & y) << 1) | (i & x);
            size_t j = k | bit;
            std::complex<float> a = dst[k];
            std::complex<float> b = dst[j] * std::conj(table[(k << shl) & mask]);
            dst[k] = a + b;
            dst[j] = a - b;
        }
    }
    return;
}

void Fourier::IFFT(std::complex<float> *dst, const std::complex<float> *src){
    std::complex<float> *table = m_Table;
    size_t               mask  = m_Mask;
    size_t               size  = m_Size;
    size_t               half  = size >> 1;
    int                  shl   = m_Bits;
    int                  shr   = 31 - shl;
    #pragma omp parallel for
    for(size_t i = 0; i < size; i++) dst[i] = src[rev32(i) >> shr]; // ビット逆転アドレッシング
    for(size_t bit = 1; shl >= 0; bit <<= 1, shl--){
        size_t x = bit - 1;
        size_t y = ~x;
        #pragma omp parallel for
        for(size_t i = 0; i < half; i++){
            size_t k = ((i & y) << 1) | (i & x);
            size_t j = k | bit;
            std::complex<float> a = dst[k];
            std::complex<float> b = dst[j] * table[(k << shl) & mask];
            dst[k] = a + b;
            dst[j] = a - b;
        }
    }
    float inv_n = 1.0f / size;
    #pragma omp parallel for
    for(size_t i = 0; i < size; i++) dst[i] *= inv_n;
    return;
}
