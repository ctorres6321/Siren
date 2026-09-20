#pragma once
#include <string>
#include <vector>
#include "dr_wav.h"

class WavReader {
public:
    explicit WavReader(const std::string& path);
    ~WavReader();

    WavReader(const WavReader&) = delete;
    WavReader& operator=(const WavReader&) = delete;

    bool readFrame(std::vector<float>& mono);
    const std::vector<float>& interleaved() const { return interleaved_; }

    bool isGenerated() const { return !loaded_; }
    float sampleRate() const { return static_cast<float>(sample_rate_); }
    unsigned channels() const { return channels_; }

private:
    drwav wav_{};
    bool loaded_ = false;
    unsigned channels_ = 1;
    unsigned sample_rate_ = 44100;

    float frequency_ = 440.0f;
    double phase_ = 0.0;

    std::vector<float> interleaved_;
};