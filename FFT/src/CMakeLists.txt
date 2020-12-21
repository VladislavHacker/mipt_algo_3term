cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)

project(FFTLib)

add_library(FFTLib fft.cpp)

project(WAV)
add_library(WAV wav.cpp)
