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

int default_add(CPU* cpu, int val1, int val2) {
    int result = val1 + val2;

    if (result == 0) CPU_SetFlag(cpu, ZF);
    else CPU_ClearFlag(cpu, ZF);

    if (result > 255) CPU_SetFlag(cpu, CF);
    else CPU_ClearFlag(cpu, CF);

    if (result % 2) CPU_SetFlag(cpu, PF);
    else CPU_ClearFlag(cpu, PF);

    if (result < 0) CPU_SetFlag(cpu, SF);
    else CPU_ClearFlag(cpu, SF);

    return result;
}

void _cmp(CPU* cpu, int val1, int val2) {
    if (val1 - val2 == 0) CPU_SetFlag(cpu, ZF);
    else CPU_ClearFlag(cpu, ZF);

    if (val1 < val2) CPU_SetFlag(cpu, CF);
    else CPU_ClearFlag(cpu, CF);

    if ((val1 - val2) < 0) CPU_SetFlag(cpu, SF);
    else CPU_ClearFlag(cpu, SF);
}

int default_sub(CPU* cpu, int val1, int val2) {
    int result = val1 - val2;

    if (result == 0) CPU_SetFlag(cpu, ZF);
    else CPU_ClearFlag(cpu, ZF);

    if (val1 < val2) CPU_SetFlag(cpu, CF);
    else CPU_ClearFlag(cpu, CF);

    if (result % 2) CPU_SetFlag(cpu, PF);
    else CPU_ClearFlag(cpu, PF);

    if (result < 0) CPU_SetFlag(cpu, SF);
    else CPU_ClearFlag(cpu, SF);

    return result;
}

int default_mul(CPU* cpu, int val1, int val2) {
    int result = val1 * val2;

    if (result == 0) CPU_SetFlag(cpu, ZF);
    else CPU_ClearFlag(cpu, ZF);

    if (result > 255) CPU_SetFlag(cpu, CF);
    else CPU_ClearFlag(cpu, CF);

    if (result % 2) CPU_SetFlag(cpu, PF);
    else CPU_ClearFlag(cpu, PF);

    if (result < 0) CPU_SetFlag(cpu, SF);
    else CPU_ClearFlag(cpu, SF);

    return result;
}

int default_div(CPU* cpu, int val1, int val2) {
    int result = val1 / val2;

    if (result == 0) CPU_SetFlag(cpu, ZF);
    else CPU_ClearFlag(cpu, ZF);

    if (result % 2) CPU_SetFlag(cpu, PF);
    else CPU_ClearFlag(cpu, PF);

    if (result < 0) CPU_SetFlag(cpu, SF);
    else CPU_ClearFlag(cpu, SF);

    return result;
}

int get_result_from_two_bytes(int high_byte, int low_byte) {
    return (high_byte << 8) + low_byte;
}

int get_low_byte_from_value(int value) {
    return value & 0xff;
}

int get_high_byte_from_value(int value) {
    return (value >> 8) & 0xff; 
}
