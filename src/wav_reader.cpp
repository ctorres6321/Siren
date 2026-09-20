#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"
#include "wav_reader.h"

#include <cmath>
#include <stdexcept>

WavReader::WavReader(const std::string& path) {
    if (path.empty()) {
        return; // sine wave mode
    }

    if (!drwav_init_file(&wav_, path.c_str(), nullptr)) {
        throw std::runtime_error("Failed to open wav file: " + path);
    }

    loaded_ = true;
    channels_ = wav_.channels;
    sample_rate_ = wav_.sampleRate;
}

WavReader::~WavReader() {
    if (loaded_) {
        drwav_uninit(&wav_);
    }
}

bool WavReader::readFrame(std::vector<float>& mono) {
    if (mono.empty()) {
        return false;
    }

    const size_t n = mono.size();

    if (!loaded_) {
        constexpr double TWO_PI = 6.283185307179586;
        const double step = TWO_PI * frequency_ / sample_rate_;
        for (size_t i = 0; i < n; ++i) {
            mono[i] = 0.8f * static_cast<float>(std::sin(phase_));
            phase_ += step;
            if (phase_ > TWO_PI) phase_ -= TWO_PI;
        }
        interleaved_ = mono;
        return true; // endless
    }

    interleaved_.assign(n * channels_, 0.0f);
    const drwav_uint64 framesRead =
        drwav_read_pcm_frames_f32(&wav_, n, interleaved_.data());

    if (framesRead == 0) {
        return false;
    }

    // Downmix to mono
    for (size_t i = 0; i < n; ++i) {
        float sum = 0.0f;
        for (unsigned c = 0; c < channels_; ++c) {
            sum += interleaved_[i * channels_ + c];
        }
        mono[i] = sum / static_cast<float>(channels_);
    }
    interleaved_.resize(framesRead * channels_);
    return true;
}