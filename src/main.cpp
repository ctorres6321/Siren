#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <string>

#include "dr_wav.h"
#include "wav_reader.h"


extern "C" {
    #include <kiss_fft.h>
    #include <kiss_fftr.h>
}

int main(int argc, char* argv[]) {
    // This can be removed
    const int N = 1024;

    // Input vector we end up working with
    std::vector<float> input(N, 0.0f);

    // We end up changing the sample rate but not frequencey, could refactor easily
    float sampleRate = 44100.0f;
    const float frequency = 440.0f;
    // GET RID OF EVENTUALLY


    WavReader reader(
        argc > 1? argv[1] : ""
    );

    if(!reader.readFrame(input)){
        reader.createSineWave(input);

        std::cout << "Using generated sine wave\n";
    }
    else {
        std::cout << "Loaded wav file\n";
    }

    kiss_fftr_cfg cfg = kiss_fftr_alloc(N, 0, nullptr, nullptr);

    if (!cfg) {
        std::cerr << "Failed to allocate FFT configuration.\n";
        return 1;
    }


    std::vector<kiss_fft_cpx> output(N / 2 + 1);
    kiss_fftr(cfg, input.data(), output.data());

    //Printing to standard output
    for (int i = 0; i < N / 2; i += 8) {
        float magnitude = std::sqrt(output[i].r * output[i].r +
                                    output[i].i * output[i].i);

        float freq = i * sampleRate / N;
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

    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    free(cfg);
    return 0;
}