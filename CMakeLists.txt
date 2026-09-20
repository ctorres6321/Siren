cmake_minimum_required(VERSION 3.16)

project(Siren LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif()

# SDL2: use the system copy if present, otherwise fetch and build it to save time for user
find_package(SDL2 QUIET)

if(SDL2_FOUND)
    message(STATUS "Using system SDL2")
    set(SDL2_TARGET SDL2::SDL2)
else()
    message(STATUS "SDL2 not found, fetching it")
    include(FetchContent)

    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON  CACHE BOOL "" FORCE)
    set(SDL_TEST   OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-2.30.9
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(SDL2)
    set(SDL2_TARGET SDL2::SDL2-static)
endif()

# Library 
add_library(audio_lib
    src/wav_reader.cpp
    src/fft.cpp
    src/visualizer.cpp
    src/audio_out.cpp

    kissfft/kiss_fft.c
    kissfft/kiss_fftr.c
    include/wav_reader.h
    include/fft.h
    include/visualizer.h
    include/audio_out.h
)

target_include_directories(audio_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/external
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/kissfft
)

target_link_libraries(audio_lib PUBLIC ${SDL2_TARGET})

if(UNIX)
    target_link_libraries(audio_lib PUBLIC m)
endif()

# Executable 
add_executable(Siren
    src/main.cpp
)

target_link_libraries(Siren PRIVATE audio_lib)