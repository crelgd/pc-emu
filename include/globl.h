#ifndef EMU_GLOBL_H
#define EMU_GLOBL_H

#ifdef EMU_EXPORTS
#define EMU_API __declspec(dllexport)
#else 
#define EMU_API __declspec(dllimport)
#endif 

#endif 