#pragma once

#include <vector>
#include <kiss_fft.h>

class Visualizer{
    public:

        explicit Visualizer(size_t fftSize);

        void render(
            const std::vector<kiss_fft_cpx>& spectrum,
            float sampleRate
        );
    private:
        size_t fftSize_;
};