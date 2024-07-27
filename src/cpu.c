/// cpu.c

/// reg[0] - r1
/// reg[1] - r2
/// reg[2] - r3
/// reg[3] - flags
///
/// R1 - register 1
/// R2 - register 2
/// R3 - register 3
/// flags - inaccessible register
///
/// <value> - (int)
/// <reg> - register (R1, R2, R3)

#include "cpu.h"
#include "other.h"
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

    memset(_cpu->flags, 0, sizeof(_cpu->flags));

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

void CPU_SetFlag(CPU* cpu, int value) {
    cpu->flags[value] = 1;
}

int CPU_CheckFlag(CPU* cpu, int value) {
    return cpu->flags[value];
}

void CPU_ClearFlag(CPU* cpu, int value) {
    cpu->flags[value] = 0;
}

void CPU_Execute(CPU* cpu) {
    int value;

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
        default_add(cpu, cpu->reg[0], value);
        cpu->pc++;
        break;

    case 0xb3: // ADD R2 <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        default_add(cpu, cpu->reg[1], value);
        cpu->pc++;
        break;

    case 0xb4: // ADD R3 <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        default_add(cpu, cpu->reg[2], value);
        cpu->pc++;
        break;

    case 0xb7: // ADD <reg> <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        if (value == 0x10) // MOV R1, R2
            default_add(cpu, cpu->reg[0], cpu->reg[1]);
        else if (value == 0x11) // MOV R1, R3
            default_add(cpu, cpu->reg[0], cpu->reg[2]);
        else if (value == 0x12) // MOV R2, R1
            default_add(cpu, cpu->reg[1], cpu->reg[0]);
        else if (value == 0x13) // MOV R2, R3
            default_add(cpu, cpu->reg[1], cpu->reg[2]);
        else if (value == 0x14) // MOV R3, R1
            default_add(cpu, cpu->reg[2], cpu->reg[0]);
        else if (value == 0x15) // MOV R3, R2
            default_add(cpu, cpu->reg[2], cpu->reg[1]);
        else cpu->run = FALSE;
        
        cpu->pc++;
        break;

    //              ALL SUB

    case 0x1e: // SUB R1, <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        default_sub(cpu, cpu->reg[0], value);
        cpu->pc++;
        break;

    case 0x2e: // SUB R2, <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        default_sub(cpu, cpu->reg[1], value);
        cpu->pc++;
        break;

    case 0x3e: // SUB R3, <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        default_sub(cpu, cpu->reg[2], value);
        cpu->pc++;
        break;

    case 0x0e: // SUB <reg> <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        if (value == 0x10) // SUB R1, R2
            default_sub(cpu, cpu->reg[0], cpu->reg[1]);
        else if (value == 0x11) // SUB R1, R3
            default_sub(cpu, cpu->reg[0], cpu->reg[2]);
        else if (value == 0x12) // SUB R2, R1
            default_sub(cpu, cpu->reg[1], cpu->reg[0]);
        else if (value == 0x13) // SUB R2, R3
            default_sub(cpu, cpu->reg[1], cpu->reg[2]);
        else if (value == 0x14) // SUB R3, R1
            default_sub(cpu, cpu->reg[2], cpu->reg[0]);
        else if (value == 0x15) // SUB R3, R2
            default_sub(cpu, cpu->reg[2], cpu->reg[1]);
        else cpu->run = FALSE;
        
        cpu->pc++;
        break;

    //              ALL MUL

    case 0x13: // MUL R1, <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        default_mul(cpu, cpu->reg[0], value);
        cpu->pc++;
        break;

    case 0x23: // MUL R2, <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        default_mul(cpu, cpu->reg[1], value);
        cpu->pc++;
        break;

    case 0x33: // MUL R3, <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        default_mul(cpu, cpu->reg[2], value);
        cpu->pc++;
        break;

    case 0x73: // MUL <reg> <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        if (value == 0x10) // SUB R1, R2
            default_mul(cpu, cpu->reg[0], cpu->reg[1]);
        else if (value == 0x11) // SUB R1, R3
            default_mul(cpu, cpu->reg[0], cpu->reg[2]);
        else if (value == 0x12) // SUB R2, R1
            default_mul(cpu, cpu->reg[1], cpu->reg[0]);
        else if (value == 0x13) // SUB R2, R3
            default_mul(cpu, cpu->reg[1], cpu->reg[2]);
        else if (value == 0x14) // SUB R3, R1
            default_mul(cpu, cpu->reg[2], cpu->reg[0]);
        else if (value == 0x15) // SUB R3, R2
            default_mul(cpu, cpu->reg[2], cpu->reg[1]);
        else cpu->run = FALSE;
        
        cpu->pc++;
        break;

    //              ALL DIV

    case 0x17: // DIV R1, <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        default_div(cpu, cpu->reg[0], value);
        cpu->pc++;
        break;

    case 0x27: // DIV R2, <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        default_div(cpu, cpu->reg[1], value);
        cpu->pc++;
        break;

    case 0x37: // DIV R3, <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        default_div(cpu, cpu->reg[2], value);
        cpu->pc++;
        break;

    case 0x47: // DIV <reg> <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        if (value == 0x10) // SUB R1, R2
            default_div(cpu, cpu->reg[0], cpu->reg[1]);
        else if (value == 0x11) // SUB R1, R3
            default_div(cpu, cpu->reg[0], cpu->reg[2]);
        else if (value == 0x12) // SUB R2, R1
            default_div(cpu, cpu->reg[1], cpu->reg[0]);
        else if (value == 0x13) // SUB R2, R3
            default_div(cpu, cpu->reg[1], cpu->reg[2]);
        else if (value == 0x14) // SUB R3, R1
            default_div(cpu, cpu->reg[2], cpu->reg[0]);
        else if (value == 0x15) // SUB R3, R2
            default_div(cpu, cpu->reg[2], cpu->reg[1]);
        else cpu->run = FALSE;
        
        cpu->pc++;
        break;

    //              ALL CMP
    case 0xf1: // CMP R1 <value>
        cpu->pc++;
        _cmp(cpu, cpu->memory[cpu->pc], cpu->reg[0]);
        cpu->pc++;
        break;

    case 0xf2: // CMP R2 <value>
        cpu->pc++;
        _cmp(cpu, cpu->memory[cpu->pc], cpu->reg[1]);
        cpu->pc++;
        break;

    case 0xf3: // CMP R3 <value>
        cpu->pc++;
        _cmp(cpu, cpu->memory[cpu->pc], cpu->reg[2]);
        cpu->pc++;
        break;

    case 0xf0: // CMP <reg> <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        if (value == 0x10) { // CMP R1, R2
            _cmp(cpu, cpu->reg[0], cpu->reg[1]);
        } else if (value == 0x11) { // CMP R1, R3
            _cmp(cpu, cpu->reg[0], cpu->reg[2]);
        } else if (value == 0x12) { // CMP R2, R1
            _cmp(cpu, cpu->reg[1], cpu->reg[0]);
        } else if (value == 0x13) { // CMP R2, R3
            _cmp(cpu, cpu->reg[1], cpu->reg[2]);
        } else if (value == 0x14) { // CMP R3, R1
            _cmp(cpu, cpu->reg[2], cpu->reg[0]);
        } else if (value == 0x15) { // CMP R3, R2
            _cmp(cpu, cpu->reg[2], cpu->reg[1]);
        } else cpu->run = FALSE;
        cpu->pc++;
        break;

    // ===== JMP
    case 0xc4: // JMP <value>
        cpu->pc++;
        cpu->pc = cpu->memory[cpu->pc];
        break;
    // ===== JIZ / JIE
    case 0xc0: // JIZ <value>
        cpu->pc++;
        if (CPU_CheckFlag(cpu, ZF) == 1)
            cpu->pc = cpu->memory[cpu->pc];
        else cpu->pc++;
        break;
    // ===== JINZ
    case 0xc1: // JINZ <value>
        cpu->pc++; 
        if (CPU_CheckFlag(cpu, ZF) != 1)
            cpu->pc = cpu->memory[cpu->pc];
        else cpu->pc++;
        break;
    // JMPS with reg
    case 0xc6: // JMP <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];

        if (value == 0x10)
            cpu->pc = cpu->reg[0]; // r1
        else if (value == 0x11)
            cpu->pc = cpu->reg[1]; // r2
        else if (value == 0x12)
            cpu->pc = cpu->reg[2]; // r3
        else cpu->pc++;

        break;

    case 0xc7: // JIZ <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];

        if (CPU_CheckFlag(cpu, ZF) == 1) {
            
            if (value == 0x10) // r1
                cpu->pc = cpu->reg[0];
            else if (value == 0x11) // r2
                cpu->pc = cpu->reg[1];
            else if (value == 0x12) // r3
                cpu->pc = cpu->reg[2];
            else cpu->pc++;

        }

        else cpu->pc++;
        break;

    case 0xc8: // JINZ <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];

        if (CPU_CheckFlag(cpu, ZF) != 1)
            if (value == 0x10) // r1
                cpu->pc = cpu->reg[0];
            else if (value == 0x11) // r2
                cpu->pc = cpu->reg[1];
            else if (value == 0x12) // r3
                cpu->pc = cpu->reg[2];
            else cpu->pc++;
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

    
    case 0x83: // CALL <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc]; // timely
        to_stack(cpu, cpu->pc + 1);
        cpu->pc = value;
        break;

    case 0x86: // CALL <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        if (value == 0x10) { // r1
            to_stack(cpu, cpu->pc + 1);
            cpu->pc = cpu->reg[0];
        }
        else if (value == 0x11) { // r2
            to_stack(cpu, cpu->pc + 1);
            cpu->pc = cpu->reg[1];
        }
        else if (value == 0x12) { // r3
            to_stack(cpu, cpu->pc + 1);
            cpu->pc = cpu->reg[2];
        }
        break;

    case 0x8d: // CALL [<value>] value by address!
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        value = cpu->memory[value];
        to_stack(cpu, cpu->pc + 1);
        cpu->pc = value;
        break;

    case 0xa1: // RET
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        if (value == 0x00)
            cpu->pc = from_stack(cpu);
        else cpu->pc++;
        break; 

    // ===== STACK

    case 0x2a: // PUSH <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        if (value == 0x10) // r1
            to_stack(cpu, cpu->reg[0]);
        else if (value == 0x11) // r2
            to_stack(cpu, cpu->reg[1]);
        else if (value == 0x12) // r3
            to_stack(cpu, cpu->reg[2]);
        cpu->pc++;
        break;

    case 0x8a: // PUSH <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        to_stack(cpu, value);
        cpu->pc++;
        break;

    case 0x5a: // POP <reg>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        if (value == 0x10) // r1
            cpu->reg[0] = from_stack(cpu);
        else if (value == 0x11) // r2
            cpu->reg[1] = from_stack(cpu);
        else if (value == 0x12) // r3
            cpu->reg[2] = from_stack(cpu);
        cpu->pc++;
        break;

    case 0x7a: // POP <value>
        cpu->pc++;
        value = cpu->memory[cpu->pc];
        cpu->memory[value] = from_stack(cpu);
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
