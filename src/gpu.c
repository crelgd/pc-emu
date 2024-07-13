#include "gpu.h"
#include <stdlib.h>
#include <string.h>

GPU* GPU_CreateGPU() {
    GPU* gpu = (GPU*)malloc(sizeof(GPU));
    memset(gpu->videomemory, 0, sizeof(gpu->videomemory));
    gpu->pixel_x = 0;
    gpu->pixel_y = 0;
    gpu->pixel_color = 0x00;
    gpu->status = 0;
    return gpu;
}

void GPU_CheckPort(CPU* cpu, GPU* gpu) {
    switch (cpu->port) {
    // 0x80 - pos X
    case 0x80:
        gpu->pixel_x = cpu->vto_port;
        break;
    // 0x81 - pos Y
    case 0x81: 
        gpu->pixel_y = cpu->vto_port;
        break;
    // 0x82 - color 
    case 0x82:
        gpu->pixel_color = cpu->vto_port;
        break;
    // 0x83 - status
    case 0x83:
        gpu->status = cpu->vto_port;
        if (gpu->status == 1) {
            GPU_Check(gpu);
            gpu->status = 0;
        }
        break;
    }
}

void GPU_Check(GPU* gpu) {
    if (gpu->pixel_x < WIDTH && gpu->pixel_y < HEIGHT) {
        gpu->videomemory[HEIGHT * gpu->pixel_y + gpu->pixel_x] = gpu->pixel_color;
    }
}

void GPU_Quit(GPU* gpu) {
    if (gpu) {
        free(gpu);
    }
}
