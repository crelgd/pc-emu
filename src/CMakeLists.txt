# EMU

set(emu_s 
    cpu.c
    emu.c 
    gpu.c 
    other.c
    rom.c
)

add_library(emu SHARED ${emu_s})
