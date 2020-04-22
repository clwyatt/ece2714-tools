cmake_minimum_required(VERSION 3.5)
project(ece2714-tools CXX)

add_executable(wav2spice
wav2spice.cpp
spice.hpp spice.cpp
wav.hpp wav.cpp
signal.hpp signal.cpp)

add_executable(spice2wav
spice2wav.cpp
spice.hpp spice.cpp
wav.hpp wav.cpp
signal.hpp signal.cpp)

