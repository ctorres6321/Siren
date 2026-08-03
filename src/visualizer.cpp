#include "visualizer.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

Visualizer::Visualizer(size_t fftSize) : fftSize_ (fftSize){}

void Visualizer::render(
    const std::vector<kiss_fft_cpx>& spectrum,
    float sampleRate
){

    for (size_t i = 0; i < spectrum.size() ; i += 8) {
        float magnitude = std::sqrt(
        spectrum[i].r * spectrum[i].r +
        spectrum[i].i * spectrum[i].i
        );

        float freq = i * sampleRate / fftSize_;
        float scaled = std::log10(1.0f + magnitude);

        int barLength = static_cast<int>(scaled * 8);
        barLength = std::clamp(barLength, 0, 40);

        std::cout << std::setw(8) << std::fixed << std::setprecision(1)
                    << freq << " Hz | ";

        for (int j = 0; j < barLength; ++j) {
            std::cout << "#";
        }

        std::cout << '\n';
    }
}