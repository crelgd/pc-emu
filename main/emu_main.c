#include <stdio.h>

#include "../emu.h"
#include "../cpu.h"
#include "../gpu.h"

#define SDL_MAIN_HANDLED
#include "include/SDL.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed: SDL initialize");
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("EMULATOR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

    SDL_Event e;
    int run = 1;
    while (run) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                run = 0;
        }

        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
