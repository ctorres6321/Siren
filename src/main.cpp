
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <string>

#include "wav_reader.h"
#include "fft.h"

int main(int argc, char* argv[]) {
    // This can be removed
    const int N = 1024;

    std::vector<float> input(N, 0.0f);

    float sampleRate = 44100.0f;
    // GET RID OF EVENTUALLY ONCE WE SEPERATE IO LOGIC 


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


    FFT fft(N);

    auto output = fft.compute(input);


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

    return 0;
}