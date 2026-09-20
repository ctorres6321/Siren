#include "wav_reader.h"
#include "fft.h"
#include "visualizer.h"

#include <SDL2/SDL.h>
#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <audio_out.h>

int main(int argc, char* argv[]) {

    // Number of samples per FFT window (also the hop size, so each frame is used once)
    constexpr size_t num_of_samples = 1024;

    try {
        if (SDL_Init(0) != 0) {
            throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
        }

        // With no argument, Siren generates a 440 Hz sine wave to use as a demo
        WavReader reader(argc > 1 ? argv[1] : "");

        std::cout << (reader.isGenerated() ? "Using generated sine wave\n" : "Loaded wav file\n");

        FFT fft(num_of_samples);
        Visualizer visualizer(num_of_samples);
        AudioOut audio(static_cast<int>(reader.sampleRate()), static_cast<int>(reader.channels()));

        // We use the Hann window to reduce spectral leakage
        std::vector<float> window(num_of_samples);
        for (size_t i = 0; i < num_of_samples; ++i) {
            window[i] = 0.5f * (1.0f - std::cos(2.0f * 3.14159265f * i / (num_of_samples - 1)));
        }

        // Keep ~3 frames of audio buffered: enough to avoid underruns, small enough that
        // the visuals stay close to what you're hearing
        const Uint32 bytesPerFrame = static_cast<Uint32>(num_of_samples * reader.channels() * sizeof(float));
        const Uint32 maxQueued = bytesPerFrame * 3;

        std::vector<float> mono(num_of_samples), windowed(num_of_samples);
        bool running = true;

        while (running && reader.readFrame(mono)) {
            audio.queue(reader.interleaved());

            for (size_t i = 0; i < num_of_samples; ++i){ 
                windowed[i] = mono[i] * window[i];
            }

            visualizer.render(fft.compute(windowed), reader.sampleRate());

            running = visualizer.pumpEvents();

            // Pace the loop to real time using the audio clock
            while (running && audio.queuedBytes() > maxQueued) {
                SDL_Delay(1);
                running = visualizer.pumpEvents();
            }
        }

        // Let the last of the audio play out (until the window is closed)
        while (running && audio.queuedBytes() > 0) {
            SDL_Delay(10);
            running = visualizer.pumpEvents();
        }
    } 
    // Catch any exceptions and print the error message
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Quit();
    return 0;
}