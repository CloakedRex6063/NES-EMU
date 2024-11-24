cmake_minimum_required(VERSION 3.30)
project(NES-EMU)

set(CMAKE_CXX_STANDARD 23)

add_executable(NES-EMU Source/main.cpp)
target_include_directories(NES-EMU PRIVATE Include)

