#include "../cpu.h"
#include <stdio.h>

int main() {
    CPU* cp = CPU_CreateCPU();

    const uint program[] = {
        0xe1, 0x01, 0xe5,
        0xe1, 0x02, 0x05,
        0xe1, 0x03, 0x08,
        0xff
    };

    CPU_LoadProgram(cp, program, sizeof(program));

    for (int i = 0; i < MEM; i++) {
        printf("%d, ", cp->memory[i]);
    }

    CPU_RunProgram(cp);

    printf("\n");

    for (int i = 0; i < 3; i++) 
        printf("%d\n", cp->reg[i]);

    CPU_Quit(cp);
    return 0;
}
