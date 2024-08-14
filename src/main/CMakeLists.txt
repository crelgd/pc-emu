# MAIN EMU

include_directories(include)

set(emu_m_s emu_main.c)

add_executable(emu ${emu_m_s})
target_link_libraries(emu ${EMU_LIB})
target_link_libraries(emu SDL2)
