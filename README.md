# Siren

A real-time audio spectrum analyzer written in C++. It plays a WAV file (or a generated sine wave) and draws its frequency content as a live bar visualizer, using an FFT for the analysis and SDL2 for graphics and audio output.

![Siren demo](extras/demo.gif)

## Features

- Loads `.wav` files, or generates a 440 Hz sine wave when no file is given
- Real-time FFT analysis with a Hann window to reduce spectral leakage
- Log-spaced frequency bands (40 Hz to 16 kHz) with dB-scaled bar heights
- Audio playback synced to the visuals, with smoothed bars and falling peak markers
- Handles mono and multi-channel files (downmixed to mono for analysis)

## How it works

1. Read a 1024-sample frame from the WAV file and downmix it to mono
2. Apply a Hann window and run a real FFT (using KissFFT)
3. Group the frequency bins into log-spaced bands and take the peak magnitude per band
4. Convert to decibels and map to bar height, with fast attack and slow decay
5. Queue the same frame to the SDL audio device and pace the loop off the audio buffer so sound and visuals stay in sync

## Tech Stack

- C++17, CMake
- [SDL2](https://www.libsdl.org/) (window, rendering, audio output)
- [KissFFT](https://github.com/mborgerding/kissfft) (FFT)
- [dr_wav](https://github.com/mackron/dr_libs) (WAV decoding)

## Build

Requires CMake 3.16+ and a C++17 compiler. SDL2 is used from your system if installed; otherwise CMake downloads and builds it automatically for convience. 

On Linux, building SDL from source needs the display and audio dev headers:

```bash
# Debian/Ubuntu
sudo apt install build-essential cmake libx11-dev libwayland-dev libasound2-dev libpulse-dev
```

```bash
cmake -S . -B build
cmake --build build
```

## Run

Sine wave demo:

```bash
./build/Siren
```

Analyze a WAV file provided (Beethoven - Fur Elise):

```bash
./build/Siren /extras/song.wav
```

Press **Esc** or close the window to quit.

## Future Improvements

- Support more audio formats (MP3, FLAC)
- Playback controls: pause, resume, repeat, playlists
- Configurable window size and band count
