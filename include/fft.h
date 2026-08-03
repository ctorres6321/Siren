#pragma once

#include <vector>

extern "C" {
    #include <kiss_fft.h>
    #include <kiss_fftr.h>
}

class FFT { 
    public: 
        explicit FFT(size_t size);

        ~FFT();

        std::vector<kiss_fft_cpx> compute(const std::vector<float>& samples);

    private:

        kiss_fftr_cfg cfg_;
        int num_of_samples = 1024;
};