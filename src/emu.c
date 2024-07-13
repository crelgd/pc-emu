#include "emu.h"
#include <stdlib.h>
#include <stdio.h>

/*
void EMU_RunEmulator(CPU* cpu, GPU* gpu) {
    while (cpu->run != FALSE) {
        if (cpu->pc >= MEM) cpu->run = FALSE;
        CPU_Execute(cpu);
        GPU_CheckPort(cpu, gpu);
    }
}
*/

const uint* EMU_GetCodeFromFile(const char* filename) {
    FILE *file;
    unsigned char buffer[1];
    uint* code = NULL;
    size_t count = 0;

    file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed: Open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    code = (uint*)malloc(file_size * sizeof(uint));
    if (code == NULL) {
        fclose(file);
        return NULL;
    }

    while (fread(buffer, sizeof(unsigned char), 1, file) == 1) {
        code[count++] = buffer[0];
    }

    fclose(file);

    return code;
}
