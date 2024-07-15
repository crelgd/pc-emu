#ifndef EMU_OTHER_H
#define EMU_OTHER_H

#include "globl.h"
#include "cpu.h"

#define STACK_SIZE 64

#ifdef __cplusplus
extern "C" {
#endif

// STACK

// Push value to stack
EMU_API void to_stack(CPU *cpu, uint value);
// Get value form stack
EMU_API uint from_stack(CPU *cpu);

#ifdef __cplusplus
}
#endif

#endif
