cmake_minimum_required(VERSION 3.5)
project(emulator)

set(SOURCES 
    cpu.c 
    gpu.c 
    emu.c 
)

add_library(emu SHARED ${SOURCES})
