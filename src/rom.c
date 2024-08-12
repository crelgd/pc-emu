#include "rom.h"
#include <stdlib.h>
#include <string.h>

ROM* ROM_InitRom() {
    ROM* rom = (ROM*)malloc(sizeof(ROM));

    rom->sector = 0;
    memset(rom->cache, -1, sizeof(rom->cache));
    rom->counter = 0;

    rom->file = NULL;

    rom->data_io = 0;
    rom->data_ram_address = 0x00;
    rom->data_sector = 0;
    rom->data_status = 0;

    return rom;
}

void ROM_ClearCache(ROM* rom) {
    memset(rom->cache, -1, sizeof(rom->cache));
    rom->counter = 0;
}

void ROM_Quit(ROM* rom) {
    if (rom) {
        free(rom);
    }
}

void ROM_WriteByte(ROM* rom, unsigned char byte) {
    if (byte > 0xff) {
        printf("BYTE: %d > 0xff(255)\n", byte);
        return;
    } if (rom->counter < 0) {
        printf("ROM COUNTER: %d < 0\n", rom->counter);
        return;
    } if (rom->counter > SECTOR_SIZE) {
        printf("ROM COUNTER %d > 0xff(255)\n", rom->counter);
        return;
    }

    rom->cache[rom->counter] = (int)byte;
    rom->counter++;
}

void ROM_DeleteByte(ROM* rom) {
    if (rom->counter < 0) {
        printf("ROM COUNTER: %d < 0\n", rom->counter);
        return;
    }

    rom->counter--;
    rom->cache[rom->counter] = -1;
}

int ROM_CheckSector(ROM* rom) { // if last byte == -1, returns 1
    if (rom->cache[SECTOR_SIZE-1] == -1) {
        return 1;
    } else return 0;
}

void ROM_SettingSector(ROM* rom, int sector) {
    if (ROM_CheckSector(rom) == 0) {
        rom->sector++;
        if (rom->sector == sector) {

        } else ROM_ClearCache(rom);
    }
}

ROM* ROM_CreateReader(ROM* rom, const char* filename) {
    rom->file = fopen(filename, "rb+");
    if (rom->file == NULL) return NULL;
    return rom;
}

void ROM_CloseReader(ROM* rom) {
    if (rom->file)
        fclose(rom->file);
}

void ROM_ReaderWriteByteToCache(ROM* rom) {
    unsigned char byte;
    if (fread(&byte, sizeof(unsigned char), 1, rom->file) == 1) {
        ROM_WriteByte(rom, byte);
    } else {
        printf("READ: Unknown error\n");
        return;
    }
}

void ROM_SectorSearch(ROM* rom, int sector, int** data) {
    while (rom->sector != sector) { // if current sector != val
        ROM_ReaderWriteByteToCache(rom); // write byte
        ROM_SettingSector(rom, sector); // sector++
    }

    memcpy(*data, rom->cache, SECTOR_SIZE * sizeof(int));
}

/*
mov r1, 1 ; i/o 1-read 2-write
out 0x16, r1

mov r1, 2 ; sector num
out 0x17, r1

mov r1, test_addr ; addr to save
out 0x18, r1

mov r1, 1 ; status (1-run, 0-none)
out 0x19, r1
*/

void ROM_CheckPort(CPU* cpu, ROM* rom) {
    switch (cpu->port)
    {
    // 0x16 - I/O
    case 0x16:
        rom->data_io = cpu->vto_port;
        break;

    // 0x17 - sector num
    case 0x17:
        rom->data_sector = cpu->vto_port;
        break;

    // 0x18 - address to save
    case 0x18:
        rom->data_ram_address = cpu->vto_port;
        break;

    // 0x19 - status
    case 0x19:
        rom->data_status = cpu->vto_port;
        if (rom->data_status == 1) {
            if (rom->data_io == 1)  { // read
                int* bfr = (int*)malloc(SECTOR_SIZE * sizeof(int));
                ROM_SectorSearch(rom, rom->data_sector, &bfr);
                for (int i = 0; i < SECTOR_SIZE; i++) {
                    cpu->memory[rom->data_ram_address + i] = bfr[i];
                }
                free(bfr);
            }
            else if (rom->data_io == 2) { // write
                int* bfr = (int*)malloc(SECTOR_SIZE * sizeof(int));
                for (int i = 0; i < SECTOR_SIZE; i++) {
                    bfr[i] = cpu->memory[rom->data_ram_address + i];
                }
                ROM_WriteToSector(rom, bfr, rom->data_sector);
                free(bfr);
            }
            rom->data_status = 0;
        }
        break;
    }
}

void ROM_WriteToSector(ROM* rom, int* value[], int sector) {
    if (sector <= 0) sector = 1; // min sector 1
    if (fseek(rom->file, (sector-1)*SECTOR_SIZE, SEEK_SET) != 0) return;

    for (int i = 0; i < SECTOR_SIZE; i++) {
        fwrite(value[i], sizeof(int), 1, rom->file);
    }
}
