#ifndef EMU_CPU_H
#define EMU_CPU_H

#include "globl.h"
#include <stddef.h>

#define TRUE 1
#define FALSE 0
typedef unsigned int uint;
#define MEM 1024
#define REG 3

typedef struct {
    uint memory[MEM];
    int reg[REG];
    int pc;
    int run;
    uint port;
} CPU;

#ifdef __cplusplus
extern "C" {
#endif 

EMU_API CPU* CPU_CreateCPU();
EMU_API void CPU_Quit(CPU* cpu);
EMU_API void CPU_LoadProgram(CPU* cpu, const uint* program, size_t size);

EMU_API void CPU_Execute(CPU* cpu);
EMU_API void CPU_RunProgram(CPU* cpu);

#ifdef __cplusplus
}
#endif 

#endif // !EMU_CPU_H
