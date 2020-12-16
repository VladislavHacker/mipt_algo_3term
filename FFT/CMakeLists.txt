cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)

project(vhWawCompressor)

include_directories(include)

add_executable(vhWawCompressor main.cpp)

add_subdirectory(src)

target_link_libraries(vhWawCompressor FFTLib)
target_link_libraries(vhWawCompressor WAV)

install(
    TARGETS vhWawCompressor
    RUNTIME DESTINATION bin
)
