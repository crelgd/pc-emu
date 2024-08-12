#ifndef EMU_ROM_H
#define EMU_ROM_H

#include "globl.h"
#include "cpu.h"

#include <stdio.h>

#define SECTOR_SIZE 128

typedef struct ROM {
    int sector;
    int cache[SECTOR_SIZE];
    int counter;
    FILE* file;

    int data_io;
    int data_sector;
    int data_ram_address;
    int data_status;
} ROM;

#ifdef __cplusplus
extern "C" {
#endif 

// init
EMU_API ROM* ROM_InitRom();

// changed cache values to -1
EMU_API void ROM_ClearCache(ROM* rom);

// clear mem
EMU_API void ROM_Quit(ROM* rom);

// write byte to cache
// 1 - if last byte in cache == null
EMU_API void ROM_WriteByte(ROM* rom, unsigned char byte);

// delete byte from cache
EMU_API void ROM_DeleteByte(ROM* rom);

// 1 - rom != SECTOR_SIZE
// 0 - ok
EMU_API int ROM_CheckSector(ROM* rom);

// sector ++
EMU_API void ROM_SettingSector(ROM* rom, int sector);

// Create file read
// NULL - file not found
EMU_API ROM* ROM_CreateReader(ROM* rom, const char* filename);

// clear file read
EMU_API void ROM_CloseReader(ROM* rom);

EMU_API void ROM_ReaderWriteByteToCache(ROM* rom);

// returns sector data
// 1) create reader
// 2) read
// 3) close reader
EMU_API void ROM_SectorSearch(ROM* rom, int sector, int** data);

EMU_API void ROM_CheckPort(CPU* cpu, ROM* rom);

EMU_API void ROM_WriteToSector(ROM* rom, int* value, int sector, int count);

#ifdef __cplusplus
}
#endif 

#endif
