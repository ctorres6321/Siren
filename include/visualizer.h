#pragma once
#include <cstddef>
#include <vector>
#include <SDL2/SDL.h>
#include "kiss_fftr.h"

class Visualizer {
public:
    Visualizer(size_t fftSize, int width = 1280, int height = 720, int numBars = 64);
    ~Visualizer();

    Visualizer(const Visualizer&) = delete;
    Visualizer& operator=(const Visualizer&) = delete;

    // Handles window events. Returns false when the user wants to quit.
    bool pumpEvents();

    void render(const std::vector<kiss_fft_cpx>& spectrum, float sampleRate);

private:
    size_t fftSize_;
    int width_, height_, numBars_;

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;

    std::vector<float> levels_; // smoothed bar heights, 0..1
    std::vector<float> peaks_;  // falling peak markers, 0..1
};