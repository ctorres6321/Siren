# Siren

A simple C++ command-line audio spectrum analyzer using Fast Fourier Transforms to display the frequency  content of the audio signal.

## Features

- Generates and analyzes a 440 Hz sine wave with no input as a demo 
- Loads `.wav` audio files as input
- Uses KissFFT for frequency analysis
- Displays a text-based frequency spectrum in the terminal
- Analyzes a middle slice of the WAV file to avoid silence at the start

## Tech Stack

- C++
- CMake
- [KissFFT](https://github.com/mborgerding/kissfft)
- [dr_libs (dr_wav.h - WAV decoding)](https://github.com/mackron/dr_libs)


## Build

```bash
cmake -S . -B build
cmake --build build --config Debug
```
## Run

With generated sine .wav:

```bash
./build/Debug/audio_spectrum_analyzer.exe
```

Analyze a WAV file:

```bash
./build/Debug/audio_spectrum_analyzer.exe sample/whitenoise.wav
```

## Tech Stack

- C++
- CMake
- [KissFFT](https://github.com/mborgerding/kissfft)
- [dr_libs (dr_wav.h - WAV decoding)](https://github.com/mackron/dr_libs)

## Future Improvements

- Add real-time audio input processing
- Visualize spectrum using a graphical interface
- Support various audio formats
