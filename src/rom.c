#include "rom.h"
#include <stdlib.h>
#include <string.h>

ROM* ROM_InitRom() {
    ROM* rom = (ROM*)malloc(sizeof(ROM));

    rom->sector = 0;
    memset(rom->cache, -1, sizeof(rom->cache));
    rom->counter = 0;

    rom->file = NULL;

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
    rom->file = fopen(filename, "rb");
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

// aдрес диска, i/o, сектор, адрес озу, статус (0-ничего не делать, 1-начать)
// addr i/o  secr aram stat
// 0x15 0x16 0x17 0x18 0x19

// TODO! port system
