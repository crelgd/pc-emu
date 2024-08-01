#ifndef EMU_ROM_H
#define EMU_ROM_H

#include "globl.h"

#define SECTOR_SIZE 128

typedef struct ROM {
    int sector;
    int cache[SECTOR_SIZE];
    int counter;
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
EMU_API int ROM_WriteByte(ROM* rom, int byte);

// delete byte from cache
EMU_API void ROM_DeleteByte(ROM* rom);

// 1 - rom != SECTOR_SIZE
// 0 - ok
EMU_API int ROM_CheckSector(ROM* rom);

// sector ++
EMU_API void ROM_SettingSector(ROM* rom);

#ifdef __cplusplus
}
#endif 

#endif
