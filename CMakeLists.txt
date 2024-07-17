cmake_minimum_required(VERSION 3.5)
project(emulator)

include_directories(include)

if (MSVC) 
    add_compile_options(/MT /O2 /Os)
endif()

add_subdirectory(src)
add_subdirectory(src/main)
