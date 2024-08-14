# EMU

set(emu_s 
    cpu.c
    emu.c 
    gpu.c 
    other.c
    rom.c
)

set(EMU_LIB emucore110)

add_library(${EMU_LIB} SHARED ${emu_s})
