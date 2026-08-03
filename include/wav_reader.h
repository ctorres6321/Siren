#pragma once

#include <vector>
#include <string>
#include "dr_wav.h"
/**
 * @brief Handles the use of reading .wav files and ensuring that the .wav file was record correctly.
 * It also handles the case of generating a sine wave if no .wav file was provided as input to the command line.
 */
class WavReader {
    public:
        explicit WavReader(const std::string& path);
        
        bool readFrame(std::vector<float>& buffer);

        float sampleRate() const;

        void createSineWave(std::vector<float>& buffer);

    private:
        drwav wav_;

        // We use these as a fallback case if IO was unable to open an input file
        // In that case we generate a sine wave using these variables

        unsigned int channels_ = 1;
        unsigned int sample_rate_ = 44100;

        static constexpr float defaultSampleRate = 44100.0f;
        static constexpr float frequency = 440.0f;
        static constexpr int N = 1024;

};