/*
reg[0] - r1
reg[1] - r2
reg[2] - r3

R1 - register 1
R2 - register 2
R3 - register 3

<value> - (int)
<reg> - register (R1, R2, R3)
*/

#include "cpu.h"
#include <stdlib.h>
#include <string.h>

CPU* CPU_CreateCPU() {
    CPU* _cpu = (CPU*)malloc(sizeof(CPU));
    memset(_cpu->memory, 0, sizeof(_cpu->memory));
    memset(_cpu->reg, 0, sizeof(_cpu->reg));
    _cpu->pc = 0;
    _cpu->run = TRUE;

    _cpu->port = 0;
    _cpu->vto_port = 0;

    return _cpu;
}

void CPU_Quit(CPU* cpu) {
    if (cpu) {
        free(cpu);
    }
}

void CPU_LoadProgram(CPU* cpu, const uint* program, size_t size) {
    memcpy(cpu->memory, program, size);
}

void CPU_Execute(CPU* cpu) {
    int value;
    int val1;
    int val2;

    switch (cpu->memory[cpu->pc]) {
    //              ALL MOV
    case 0xe1: // MOV R1 <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        cpu->reg[0] = value;
        cpu->pc++;
        break;

    case 0xe2: // MOV R2 <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        cpu->reg[1] = value;
        cpu->pc++;
        break;

    case 0xe3: // MOV R3 <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        cpu->reg[2] = value;
        cpu->pc++;
        break;
    
    case 0xe0: // MOV <reg> <reg> 
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        if (value == 0x10) // MOV R1, R2
            cpu->reg[0] = cpu->reg[1];
        else if (value == 0x11) // MOV R1, R3
            cpu->reg[0] = cpu->reg[2];
        else if (value == 0x12) // MOV R2, R1
            cpu->reg[1] = cpu->reg[0];
        else if (value == 0x13) // MOV R2, R3
            cpu->reg[1] = cpu->reg[2];
        else if (value == 0x14) // MOV R3, R1
            cpu->reg[2] = cpu->reg[0];
        else if (value == 0x15) // MOV R3, R2
            cpu->reg[2] = cpu-> reg[1];
        else cpu->run = FALSE;
        
        cpu->pc++;
        break;

    case 0xe4: // MOV <value> R1
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        cpu->memory[value] = cpu->reg[0];
        cpu->pc++;
        break;

    case 0xe5: // MOV <value> R2
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        cpu->memory[value] = cpu->reg[1];
        cpu->pc++;
        break;

    case 0xe6: // MOV <value> R3
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        cpu->memory[value] = cpu->reg[2];
        cpu->pc++;
        break;

    //              ALL ADD
    case 0xb2: // ADD R1 <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        cpu->reg[0] += value;
        cpu->pc++;
        break;

    case 0xb3: // ADD R2 <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        cpu->reg[1] += value;
        cpu->pc++;
        break;

    case 0xb4: // ADD R3 <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        cpu->reg[2] += value;
        cpu->pc++;
        break;

    case 0xb7: // ADD <reg> <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        if (value == 0x10) // MOV R1, R2
            cpu->reg[0] += cpu->reg[1];
        else if (value == 0x11) // MOV R1, R3
            cpu->reg[0] += cpu->reg[2];
        else if (value == 0x12) // MOV R2, R1
            cpu->reg[1] += cpu->reg[0];
        else if (value == 0x13) // MOV R2, R3
            cpu->reg[1] += cpu->reg[2];
        else if (value == 0x14) // MOV R3, R1
            cpu->reg[2] += cpu->reg[0];
        else if (value == 0x15) // MOV R3, R2
            cpu->reg[2] += cpu-> reg[1];
        else cpu->run = FALSE;
        
        cpu->pc++;
        break;

    //              ALL CMP
    case 0xf1: // CMP R1 <value>
        cpu->pc++;
        val1 = cpu->memory[cpu->pc];
        val2 = cpu->reg[0];
        cpu->pc++;
        break;

    case 0xf2: // CMP R2 <value>
        cpu->pc++;
        val1 = cpu->memory[cpu->pc];
        val2 = cpu->reg[1];
        cpu->pc++;
        break;

    case 0xf3: // CMP R3 <value>
        cpu->pc++;
        val1 = cpu->memory[cpu->pc];
        val2 = cpu->reg[2];
        cpu->pc++;
        break;

    case 0xf0: // CMP <reg> <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        if (value == 10) { // CMP R1, R2
            val1 = cpu->reg[0];
            val2 = cpu->reg[1];
        } else if (value == 11) { // CMP R1, R3
            val1 = cpu->reg[0];
            val2 = cpu->reg[2];
        } else if (value == 12) { // CMP R2, R1
            val1 = cpu->reg[1];
            val2 = cpu->reg[0];
        } else if (value == 13) { // CMP R2, R3
            val1 = cpu->reg[1];
            val2 = cpu->reg[2];
        } else if (value == 14) { // CMP R3, R1
            val1 = cpu->reg[2];
            val2 = cpu->reg[0];
        } else if (value == 15) { // CMP R3, R2
            val1 = cpu->reg[2];
            val2 = cpu->reg[1];
        } else cpu->run = FALSE;
        cpu->pc++;
        break;

    // ===== JMP
    case 0xc4: // JMP <value>
        cpu->pc++;
        cpu->pc = cpu->memory[cpu->pc];
        break;
    // ===== JIZ
    case 0xc0: // JIZ <value>
        cpu->pc++;
        if (val2==0) val1 = 0; 
        if (val1 == 0)
            cpu->pc = cpu->memory[cpu->pc];
        else cpu->pc++;
        break;
    // ===== JINZ
    case 0xc1: // JINZ <value>
        cpu->pc++;
        if (val2==0) val1 = 0; 
        if (val1 != 0)
            cpu->pc = cpu->memory[cpu->pc];
        else cpu->pc++;
        break;
    // ===== JIE
    case 0xc2: // JIE <value>
        cpu->pc++;
        if (val1 == val2)
            cpu->pc = cpu->memory[cpu->pc];
        else cpu->pc++;
        break;

    // ======= OUT
    case 0xd1: // OUT <value> R1
        cpu->pc++;
        cpu->port = cpu->memory[cpu->pc];
        cpu->vto_port = cpu->reg[0];
        cpu->pc++;
        break;
    case 0xd2: // OUT <value> R2
        cpu->pc++;
        cpu->port = cpu->memory[cpu->pc];
        cpu->vto_port = cpu->reg[1];
        cpu->pc++;
        break;
    case 0xd3: // OUT <value> R3
        cpu->pc++;
        cpu->port = cpu->memory[cpu->pc];
        cpu->vto_port = cpu->reg[2];
        cpu->pc++;
        break;

    // END==================================
    case 0xff: // END 
        cpu->run = FALSE;
        break;

    default:
        cpu->pc++;
        break;
    }
}

void CPU_RunProgram(CPU* cpu) {
    while (cpu->run != FALSE) {
        if (cpu->pc >= MEM) cpu->run = FALSE;
        CPU_Execute(cpu);
    }
}
