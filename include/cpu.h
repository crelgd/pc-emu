#ifndef EMU_CPU_H
#define EMU_CPU_H

#include "globl.h"
#include <stddef.h>

#define TRUE 1
#define FALSE 0
typedef unsigned int uint;
#define REG 4 // info in cpu.c
#define MEM 1000000

#define ZF 0 // if zero
#define CF 1 // if > 255
#define PF 2 // if <value> / 2
#define SF 3 // if <value> < 0 (have -)

/*!TODO
sub (if result < 0 CF = 1)
*/

typedef struct {
    int memory[MEM];
    int reg[REG];
    int pc;
    int run;
    uint port;
    uint vto_port;
    int flags[4];
} CPU;

#ifdef __cplusplus
extern "C" {
#endif 

EMU_API CPU* CPU_CreateCPU();
EMU_API void CPU_Quit(CPU* cpu);
EMU_API void CPU_LoadProgram(CPU* cpu, const uint* program, size_t size);

EMU_API void CPU_Execute(CPU* cpu);
EMU_API void CPU_RunProgram(CPU* cpu);

// set flag value to 1
EMU_API void CPU_SetFlag(CPU* cpu, int value);
// check flag value
EMU_API int CPU_CheckFlag(CPU* cpu, int value);
// set flag to 0
EMU_API void CPU_ClearFlag(CPU* cpu, int value);

#ifdef __cplusplus
}
#endif 

#endif // !EMU_CPU_H
