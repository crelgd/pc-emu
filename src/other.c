#include "other.h"

int offset = 0;

void to_stack(CPU *cpu, uint value) {
    if (offset >= STACK_SIZE) {
        return;
    }

    cpu->memory[MEM - 1 - offset] = value;
    offset++;
}

uint from_stack(CPU *cpu) {
    if (offset == 0) {
        return;
    }

    offset--;
    return cpu->memory[MEM - 1 - offset];
}

void default_add(CPU* cpu, int val1, int val2) {
    val1 += val2;

    if (val1 == 0) CPU_SetFlag(cpu, ZF);
    else CPU_ClearFlag(cpu, ZF);

    if (val1 > 255) CPU_SetFlag(cpu, CF);
    else CPU_ClearFlag(cpu, CF);

    if (val1 % 2) CPU_SetFlag(cpu, PF);
    else CPU_ClearFlag(cpu, PF);

    if (val1 < 0) CPU_SetFlag(cpu, SF);
    else CPU_ClearFlag(cpu, SF);
}

void _cmp(CPU* cpu, int val1, int val2) {
    if (val1 - val2 == 0) CPU_SetFlag(cpu, ZF);
    else CPU_ClearFlag(cpu, ZF);

    if (val1 < val2) CPU_SetFlag(cpu, CF);
    else CPU_ClearFlag(cpu, CF);

    if ((val1 - val2) < 0) CPU_SetFlag(cpu, SF);
    else CPU_ClearFlag(cpu, SF);
}

void default_sub(CPU* cpu, int val1, int val2) {
    val1 -= val2;

    if (val1 == 0) CPU_SetFlag(cpu, ZF);
    else CPU_ClearFlag(cpu, ZF);

    if (val1 < val2) CPU_SetFlag(cpu, CF);
    else CPU_ClearFlag(cpu, CF);

    if (val1 % 2) CPU_SetFlag(cpu, PF);
    else CPU_ClearFlag(cpu, PF);

    if (val1 < 0) CPU_SetFlag(cpu, SF);
    else CPU_ClearFlag(cpu, SF);
}
