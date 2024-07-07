#include <stdio.h>

#include "../emu.h"
#include "../cpu.h"
#include "../gpu.h"

#define SDL_MAIN_HANDLED
#include "include/SDL.h"

int main(int argc, char* argv[]) {
    if (argc < 2) return 0;

    const char* filename = argv[1];

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed: SDL initialize");
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("EMULATOR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

    SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    CPU *cpu = CPU_CreateCPU();
    GPU* gpu = GPU_CreateGPU();

    const uint* program = EMU_GetCodeFromFile(filename);

    int program_size = 0;
    while (program[program_size] != 0x4d) {
        program_size++;
    }

    CPU_LoadProgram(cpu, program, program_size * sizeof(uint));

    SDL_Event e;

    for (int i = 0; i < program_size; i++)
        printf("%d, ", cpu->memory[i]);

    printf("\n\n\n");

    while (cpu->run != FALSE) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                cpu->run = FALSE;
        }

        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);

        // RUN EMU
        if (cpu->pc >= MEM) cpu->run = FALSE;
        CPU_Execute(cpu);
        GPU_CheckPort(cpu, gpu);

        printf("R1: %d | R2: %d | R3: %d\n", cpu->reg[0], cpu->reg[1], cpu->reg[2]);

        // DRAW PIXELS
        for (int i = 0; i < WIDTH*HEIGHT; i++) { 
            int x = i % WIDTH;
            int y = i / WIDTH;

            if (gpu->videomemory[i] != 0) SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
            else SDL_SetRenderDrawColor(render, 0, 0, 0, 255);

            SDL_RenderDrawPoint(render, x, y);
        }

        SDL_RenderPresent(render);

        SDL_Delay(1);
    }

    GPU_Quit(gpu);
    CPU_Quit(cpu);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
