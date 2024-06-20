#ifndef EMU_GPU_H
#define EMU_GPU_H

#include "cpu.h"

/*
CPU PROGRAM EXAMPLE:
mov r1, 3 ; X pos
mov r2, 2 ; Y pos
out 0x80, r1
out 0x81, r2
mov r1, 0x00 ; Color WHITE
out 0x82, r1
mov r1, 1 ; STATUS: draw
out 0x83, r1

5x3
00000
00100
00000
*/

#define WIDTH 160
#define HEIGHT 120

typedef struct {
    uint videomemory[WIDTH*HEIGHT];
    uint pixel_x; 
    uint pixel_y;
    uint pixel_color;
    uint status; // if status 1, draw pixel
} GPU;

#ifdef __cplusplus
extern "C" {
#endif 

EMU_API GPU* GPU_CreateGPU();
EMU_API void GPU_CheckPort(CPU* cpu, GPU* gpu);
EMU_API void GPU_Quit(GPU* gpu);

#ifdef __cplusplus
}
#endif 

#endif 
