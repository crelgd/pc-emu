#ifndef EMU_MAIN_H
#define EMU_MAIN_H

#include "cpu.h"
//#include "gpu.h"

#ifdef __cplusplus
extern "C" {
#endif 

//EMU_API void EMU_RunEmulator(CPU* cpu, GPU* gpu);
EMU_API const uint* EMU_GetCodeFromFile(const char* filename);

#ifdef __cplusplus
}
#endif 

#endif 
