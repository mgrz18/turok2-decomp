#ifndef STRUCTS_GBI_H
#define STRUCTS_GBI_H

/* libultra graphics types (PR/gbi.h, PR/mbi.h) at the size the game's
 * structs embed them, opaque where the engine code only passes them on. */

#include "common.h"

typedef struct { u8 m[0x40]; } Mtx;          /* 4x4 fixed-point matrix */

typedef struct {
    /* 0x0 */ s16 vscale[4];
    /* 0x8 */ s16 vtrans[4];
} Vp; /* size 0x10 */

typedef struct { u8 l[0x20]; } LookAt;       /* two Light_t directions */

/* A vertex (PR/gbi.h Vtx_t). */
typedef struct {
    /* 0x0 */ s16 ob[3];        /* position */
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];        /* texture coordinates */
    /* 0xC */ u8 cn[4];         /* colour and alpha */
} Vtx_t; /* size 0x10 */

#endif
