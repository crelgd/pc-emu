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
// add instruction
EMU_API int default_add(CPU* cpu, int val1, int val2);

EMU_API void _cmp(CPU* cpu, int val1, int val2);

EMU_API int default_sub(CPU* cpu, int val1, int val2);

EMU_API int default_mul(CPU* cpu, int val1, int val2);

EMU_API int default_div(CPU* cpu, int val1, int val2);

EMU_API int get_result_from_two_bytes(int high_byte, int low_byte);

EMU_API int get_low_byte_from_value(int value);
EMU_API int get_high_byte_from_value(int value);

#ifdef __cplusplus
}
#endif

#endif
