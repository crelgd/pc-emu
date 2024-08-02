#include "rom.h"
#include <stdlib.h>
#include <string.h>

ROM* ROM_InitRom() {
    ROM* rom = (ROM*)malloc(sizeof(ROM));

    rom->sector = -1;
    memset(rom->cache, -1, sizeof(rom->cache));
    rom->counter = 0;

    rom->file = NULL;

    rom->last_byte[0] = 0x00;

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

int ROM_WriteByte(ROM* rom, int byte) {
    if (byte >= 0xff) {
        printf("BYTE: %d > 0xff(255)", byte);
        return;
    } if (rom->counter < 0) {
        printf("ROM COUNTER: %d < 0", rom->counter);
        return;
    } if (rom->counter >= 0xff) {
        printf("ROM COUNTER %d > 0xff(255)", rom->counter);
        return;
    }
    if (ROM_CheckSector(rom) == 1)
        return 1;
    else return 0;

    rom->cache[rom->counter] = byte;
    rom->counter++;
}

void ROM_DeleteByte(ROM* rom) {
    if (rom->counter < 0) {
        printf("ROM COUNTER: %d < 0", rom->counter);
        return;
    }

    rom->cache[rom->counter] = -1;
    rom->counter--;
}

int ROM_CheckSector(ROM* rom) { // if last byte == -1, returns 1
    if (rom->cache[SECTOR_SIZE-1] != -1) {
        return 0;
    } else return 1;
}

void ROM_SettingSector(ROM* rom) {
    if (ROM_CheckSector(rom) != 1) {
        rom->sector++;
        ROM_ClearCache(rom);
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
    if (fread(rom->last_byte, sizeof(int), 1, rom->file) == 1) {
        ROM_WriteByte(rom, rom->last_byte[0]);
    }
}

void ROM_SectorAdder(ROM* rom, int sector) {
    while (rom->sector != sector) { // if current sector != val
        ROM_ReaderWriteByteToCache(rom); // write byte
        ROM_SettingSector(rom); // sector++
    }
}

// aдрес диска, i/o, сектор, адрес озу, статус (0-ничего не делать, 1-начать)
// addr i/o  secr aram stat
// 0x15 0x16 0x17 0x18 0x19

// TODO! port system
