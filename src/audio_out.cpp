#include "audio_out.h"
#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>

    AudioOut::AudioOut(int sampleRate, int channels) {

        if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
            throw std::runtime_error(std::string("SDL audio init failed: ") + SDL_GetError());
        }

        // Configure the audio specification
        SDL_AudioSpec want{}, have{};
        want.freq = sampleRate;
        want.format = AUDIO_F32SYS;
        want.channels = static_cast<Uint8>(channels);
        want.samples = 1024;
        want.callback = nullptr; // queue mode

        dev_ = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0); // SDL converts if needed
        if (dev_ == 0) {
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
            throw std::runtime_error(std::string("SDL_OpenAudioDevice failed: ") + SDL_GetError());
        }
        SDL_PauseAudioDevice(dev_, 0);
    }

    AudioOut::~AudioOut() {
        SDL_CloseAudioDevice(dev_);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }

    void AudioOut::queue(const std::vector<float>& samples) {
        SDL_QueueAudio(dev_, samples.data(), static_cast<Uint32>(samples.size() * sizeof(float)));
    }

