#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"
#include "wav_reader.h"

#include <iostream>
#include <cmath>
#include <stdexcept>

WavReader::WavReader(const std::string& path){
    if(!drwav_init_file(
        &wav_,
        path.c_str(),
        nullptr))
    {
        throw std::runtime_error(
            "Failed to open wav file"
        );
    }
    channels_ = wav_.channels;
    sample_rate_ = wav_.sampleRate;
}

// If readFrame returns false then we fill in the input buffer with a generated sine wave and return it
void WavReader::createSineWave(std::vector<float>& buffer){

    constexpr float PI = 3.14159265358979323846f;

    for (int i = 0; i < buffer.size(); i++) {
        buffer[i] = 
            std::sin(2.0f * static_cast<float>(PI) * frequency * i / defaultSampleRate
        );
    }

}

bool WavReader::readFrame(std::vector<float>& buffer){

    if (buffer.empty()){
        return false;
    }

    drwav_uint64 frames_read = 
        drwav_read_pcm_frames_f32(
            &wav_,
            buffer.size(),
            buffer.data()
        );
    
    // If we read any frames then we return true
    return  frames_read > 0;

}

float WavReader::sampleRate() const {
     return static_cast<float> (sample_rate_); 
}
