#ifndef STRUCTS_MEMORY_H
#define STRUCTS_MEMORY_H

/* The debugger's memory permission map (the host I/O code, Rage Wars'
 * src/IO): D_80128130 holds 16 ranges of RAM (func_002A2104 builds them
 * for the installed RAM, func_002A1DEC checks host reads and writes
 * against them) and then the 32 mapped TLB ranges (func_002A13F0).
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

/* m_Flags: 3 valid (TLB ranges); 4 and 8 pick how func_002A1DEC copies. */
typedef struct CMemoryRange {
    /* 0x0 */ u32 m_First;                  /* -1: unused */
    /* 0x4 */ u32 m_Last;
    /* 0x8 */ u32 m_Flags;
} CMemoryRange; /* size 0xC */

extern CMemoryRange D_80128130[48];     /* 16 memory ranges, then the 32 TLB ranges */

#endif
