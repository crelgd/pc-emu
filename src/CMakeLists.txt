# EMU

if (MSVC) 
    add_compile_options(/MT /O2 /Os)
endif()

set(emu_s 
    cpu.c
    emu.c 
    gpu.c 
)

add_library(emu SHARED ${emu_s})
