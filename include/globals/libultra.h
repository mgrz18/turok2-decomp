#ifndef GLOBALS_LIBULTRA_H
#define GLOBALS_LIBULTRA_H

/* libultra globals, with ultralib's own types. */

#include "common.h"

/* The boot parameters the IPL3 leaves in low memory (ultralib parameters.s). */
extern s32 osTvType;            /* 0 PAL, 1 NTSC, 2 MPAL */
extern void *osRomBase;         /* cart base address for PI DMA */
extern s32 osResetType;         /* 0 cold boot, 1 NMI */
extern u32 osMemSize;
extern u8 osAppNMIBuffer[64];

#endif
