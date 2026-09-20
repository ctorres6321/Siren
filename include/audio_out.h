#pragma once

#include <SDL2/SDL.h>
#include <vector>

class AudioOut {
public:

    AudioOut(int sampleRate, int channels);
    ~AudioOut();

    // Delete copy constructor and assignment operator to prevent copying
    AudioOut(const AudioOut&) = delete;
    AudioOut& operator=(const AudioOut&) = delete;

    // Queue audio samples for playback
    void queue(const std::vector<float>& samples);
    
    // Returns the number of bytes currently queued in the audio device
    Uint32 queuedBytes() const { return SDL_GetQueuedAudioSize(dev_); }

private:
    // Our SDL audio device ID
    SDL_AudioDeviceID dev_ = 0;
};
