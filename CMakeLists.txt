cmake_minimum_required(VERSION 3.5)
project(emulator)

set(SOURCES 
    cpu.c 
    gpu.c 
    emu.c 
)

set(UI_SOR
    main/emu_main.c
)

add_library(emu SHARED ${SOURCES})

link_directories(build)

add_executable(main_emu ${UI_SOR})
target_link_libraries(main_emu SDL2)
