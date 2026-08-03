cmake_minimum_required(VERSION 3.10)

project(AudioSpectrumAnalyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Library
add_library(audio_lib
    src/wav_reader.cpp
    src/fft.cpp
    src/visualizer.cpp
    include/wav_reader.h
    include/fft.h
    include/visualizer.h
)

target_include_directories(audio_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/external
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/kissfft
)


# Executable
add_executable(audio_spectrum_analyzer
    src/main.cpp
    kissfft/kiss_fft.c
    kissfft/kiss_fftr.c
)

target_include_directories(audio_spectrum_analyzer PRIVATE
    ${PROJECT_SOURCE_DIR}/kissfft
)


target_link_libraries(audio_spectrum_analyzer
    PRIVATE audio_lib
)