#ifndef GLOBALS_UCODE_H
#define GLOBALS_UCODE_H

/* The RSP microcode the game links (PR/rspboot.h, gspF3DEX, gspL3DEX, aspMain), 0x800BC330-0x800BFF50:
 * text and data starts, typed u64[] as the SDK declares them. */

#include "common.h"

extern u64 rspbootTextStart[];
extern u64 rspbootTextEnd[];            /* the address of gspF3DEX_NoN_fifoTextStart, a linker symbol */
extern u64 gspF3DEX_NoN_fifoTextStart[]; /* also where rspboot's text ends */
extern u64 gspL3DEX_fifoTextStart[];
extern u64 aspMainTextStart[];
extern u64 gspF3DEX_NoN_fifoDataStart[]; /* "RSP Gfx ucode F3DEX.NoN fifo 2.05" */
extern u64 gspL3DEX_fifoDataStart[];     /* "RSP Gfx ucode L3DEX fifo 2.05" */
extern u64 aspMainDataStart[];

#endif
