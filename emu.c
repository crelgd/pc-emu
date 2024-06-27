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

const uint* EMU_GetCodeFromFile(const char* filename, size_t* out_size) {
    FILE *file;
    uint* code = NULL;
    size_t count = 0;

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    code = (int*)malloc(file_size * sizeof(int));
    if (code == NULL) {
        fclose(file);
        return NULL;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        code[count++] = ch;
    }

    fclose(file);

    if (out_size != NULL) {
        *out_size = count;
    }

    return code;
}
