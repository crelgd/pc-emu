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
