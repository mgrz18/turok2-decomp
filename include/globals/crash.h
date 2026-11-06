#ifndef GLOBALS_CRASH_H
#define GLOBALS_CRASH_H

/* The crash screen (debug console), 0x800B6EA0-0x800B75EC. */

#include "common.h"

/* A named flag of a register (Crash_PrintFlags), as Nintendo's fault example's causeDesc. */
typedef struct FlagDesc {
    /* 0x0 */ u32 mask;
    /* 0x4 */ u32 value;
    /* 0x8 */ u8 *name;
} FlagDesc; /* size 0xC */

extern u16 *sCrashFrameBuffer;        /* the frame buffer the crash screen draws into */
extern u8 gCrashFont[0x60][16];       /* 8x16 font for ASCII 0x20-0x7F (Crash_PutChar) */
extern u32 sCrashCursorX;             /* console column (2) */
extern u32 sCrashCursorY;             /* console row */
extern u16 sCrashBgColor;             /* background colour, RGBA5551 */
extern FlagDesc gCrashCauseDesc[28];  /* the cause register's flags and exception codes, NULL-terminated */
extern volatile s32 sCrashCanExit;    /* set when the crash screen may be left */

#endif
