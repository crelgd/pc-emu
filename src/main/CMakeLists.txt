# MAIN EMU

include_directories(include)

set(emu_m_s emu_main.c)

add_executable(emu_main ${emu_m_s})
target_link_libraries(emu_main emu)
target_link_libraries(emu_main SDL2)
