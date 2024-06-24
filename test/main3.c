#include <stdio.h>
#include "../cpu.h"
#include "../gpu.h"

int main() {
    CPU* cpu = CPU_CreateCPU();
    GPU* gpu = GPU_CreateGPU();

    const int program[]  = {
        0xe1, 0x03, // mov r1, 3
        0xd1, 0x80, // out 0x80, r1 
        0xe1, 0x05, // mov r1, 5
        0xd1, 0x81, // out 0x81, r1
        0xe1, 0x07, // mov r1, 7
        0xd1, 0x82, // out 0x82, r1
        0xe1, 0x01, // mov r1, 1
        0xd1, 0x83, // out 0x83, r1
        0xff // end
    };

    CPU_LoadProgram(cpu, program, sizeof(program));

    while (cpu->run != FALSE) {
        if (cpu->pc >= MEM) cpu->run = FALSE;
        CPU_Execute(cpu);
        GPU_CheckPort(cpu, gpu);
    }
    
    printf("Video Memory:\n");
    for (int i = 0; i < WIDTH*HEIGHT; i++) {
        printf("%d", gpu->videomemory[i]);
    }

    GPU_Quit(gpu);
    CPU_Quit(cpu);
    return 0;
}
