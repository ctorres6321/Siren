#include "wav_reader.h"
#include "fft.h"
#include "visualizer.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    // N represents the numbers of samples processed per FFT window

    const int N = 1024;

    /*
        Attempt to read the wav file with WavReader, if no arguement is 
        passed in then we fall back and generate the sine wave and read from there.
    */
    
    std::vector<float> input(N, 0.0f);

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

    /*
        Attempt to then transform the read in wav file with 
        KissFFT and with the amount of samples passed into our wrapper class.
    */

    FFT fft(N);
    Visualizer visualizer(N);

    while(reader.readFrame(input)){
        auto spectrum = fft.compute(input);

        visualizer.render(
            spectrum,
            reader.sampleRate()
        );

        std::this_thread::sleep_for(
            std::chrono::microseconds(600)
        );
    }

    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0;
}