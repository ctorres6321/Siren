#include "fft.h"
#include <stdexcept>

FFT::FFT(size_t size)
    : num_of_samples(size) { 
    cfg_ = kiss_fftr_alloc(
        num_of_samples, 0, nullptr, nullptr
    );

    if(!cfg_) { 
        throw std::runtime_error(
            "Failed to allocate FFT configuration"
        );

    }
}

FFT::~FFT(){
    free(cfg_);
}

std::vector<kiss_fft_cpx> FFT::compute(const std::vector<float>& samples){

    std::vector<kiss_fft_cpx> output(num_of_samples / 2 + 1);

    kiss_fftr(cfg_, samples.data(), output.data());

    return output;
}