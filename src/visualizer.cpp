#include "visualizer.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>
 
namespace {
 
// h in [0,360), s and v in [0,1]
SDL_Color hsv(float h, float s, float v) {
    const float c = v * s;
    const float x = c * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
    const float m = v - c;
    float r = 0, g = 0, b = 0;
    if      (h < 60)  { r = c; g = x; }
    else if (h < 120) { r = x; g = c; }
    else if (h < 180) { g = c; b = x; }
    else if (h < 240) { g = x; b = c; }
    else if (h < 300) { r = x; b = c; }
    else              { r = c; b = x; }
    return { static_cast<Uint8>((r + m) * 255), static_cast<Uint8>((g + m) * 255),
             static_cast<Uint8>((b + m) * 255), 255 };
}
 
} // namespace
 
Visualizer::Visualizer(size_t fftSize, int width, int height, int numBars)
    : fftSize_(fftSize), width_(width), height_(height), numBars_(numBars),
      levels_(numBars, 0.0f), peaks_(numBars, 0.0f) {
 
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(std::string("SDL video init failed: ") + SDL_GetError());
    }
 
    window_ = SDL_CreateWindow("Siren", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, width_, height_, SDL_WINDOW_SHOWN);
    if (!window_) {
        throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
    }
 
    renderer_ = SDL_CreateRenderer(window_, -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
        renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_SOFTWARE);
    }
    if (!renderer_) {
        throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());
    }
}
 
Visualizer::~Visualizer() {
    if (renderer_) SDL_DestroyRenderer(renderer_);
    if (window_) SDL_DestroyWindow(window_);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
 
bool Visualizer::pumpEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) return false;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return false;
    }
    return true;
}
 
void Visualizer::render(const std::vector<kiss_fft_cpx>& spectrum, float sampleRate) {
    SDL_SetRenderDrawColor(renderer_, 12, 12, 18, 255);
    SDL_RenderClear(renderer_);
 
    if (spectrum.empty()) {
        SDL_RenderPresent(renderer_);
        return;
    }
 
    const float nyquist = sampleRate * 0.5f;
    const float fMin = 40.0f;
    const float fMax = std::min(16000.0f, nyquist * 0.98f);
    const float ratio = fMax / fMin;
 
    // A full-scale sine under a Hann window peaks at about N/4
    const float norm = static_cast<float>(fftSize_) / 4.0f;
 
    const int margin = 20;
    const int maxBarH = height_ - 2 * margin;
    const int segH = 8;   // LED-style segment height (including gap)
    const int segments = maxBarH / segH;
    const float barW = static_cast<float>(width_ - 2 * margin) / numBars_;
 
    const size_t lastBin = spectrum.size() - 1;
 
    for (int b = 0; b < numBars_; ++b) {
        // Log-spaced frequency range for this bar
        const float f0 = fMin * std::pow(ratio, static_cast<float>(b) / numBars_);
        const float f1 = fMin * std::pow(ratio, static_cast<float>(b + 1) / numBars_);
 
        size_t lo = static_cast<size_t>(f0 * fftSize_ / sampleRate);
        size_t hi = static_cast<size_t>(std::ceil(f1 * fftSize_ / sampleRate));
        lo = std::min(lo, lastBin);
        hi = std::clamp(hi, lo, lastBin);
 
        float peakMag = 0.0f;
        for (size_t k = lo; k <= hi; ++k) {
            const float mag = std::sqrt(spectrum[k].r * spectrum[k].r +
                                        spectrum[k].i * spectrum[k].i);
            peakMag = std::max(peakMag, mag);
        }
 
        // dB scale: -70 dB -> 0, 0 dB -> 1
        const float db = 20.0f * std::log10(peakMag / norm + 1e-7f);
        const float target = std::clamp((db + 70.0f) / 70.0f, 0.0f, 1.0f);
 
        // Instant attack, gradual release
        levels_[b] = (target > levels_[b]) ? target : std::max(target, levels_[b] - 0.03f);
        peaks_[b] = std::max(levels_[b], peaks_[b] - 0.006f);
 
        const int x = margin + static_cast<int>(b * barW);
        const int w = std::max(1, static_cast<int>(barW) - 2);
        const int lit = static_cast<int>(levels_[b] * segments);
 
        for (int s = 0; s < lit; ++s) {
            const float t = static_cast<float>(s) / segments;
            const SDL_Color c = hsv(220.0f * (1.0f - t), 0.85f, 1.0f); // blue -> red
            SDL_SetRenderDrawColor(renderer_, c.r, c.g, c.b, 255);
            SDL_Rect r{ x, height_ - margin - (s + 1) * segH + 2, w, segH - 2 };
            SDL_RenderFillRect(renderer_, &r);
        }
 
        // Peak marker
        const int peakSeg = std::min(segments - 1, static_cast<int>(peaks_[b] * segments));
        if (peakSeg >= lit && peaks_[b] > 0.02f) {
            SDL_SetRenderDrawColor(renderer_, 235, 235, 245, 255);
            SDL_Rect r{ x, height_ - margin - (peakSeg + 1) * segH + 2, w, 2 };
            SDL_RenderFillRect(renderer_, &r);
        }
    }
 
    SDL_RenderPresent(renderer_);
}
