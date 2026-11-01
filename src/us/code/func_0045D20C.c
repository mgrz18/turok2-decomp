/* Second-module copy of func_004242EC (first module). The mode record is read as an s32 array so the loop reloads its width (alias ordering). */
#include "common.h"

s32 func_00266B80(void); /* osGetMemSize */

#define MAX(a, b) ((a) >= (b) ? (a) : (b))

typedef struct {
    /* 0x00 */ s32 width;
    /* 0x04 */ s32 height;
    /* 0x08 */ u8 pad8[0x14];
} VideoMode; /* size 0x1C */

extern s32 D_80000300; /* osTvType */
extern s32 D_800C2030;
extern s32 D_800C2034;
extern s32 D_800C2038; /* current mode */
extern s32 D_800C203C; /* previous mode */
extern s32 D_800C204C[2]; /* mode count per TV region */
extern VideoMode D_800C2054[2][5];

/* Steps to the next video mode of the TV region that fits in memory. */
void func_0045D20C(void) {
    s32 region = 0;
    s32 *mode;
    s32 limit;

    switch (D_80000300) {
        case 0:
        case 2:
            region = 1;
            break;
        case 1:
            break;
    }
    mode = (s32 *)&D_800C2054[region][D_800C2038];
    if (func_00266B80() != 0x400000) {
        limit = 0x2A300;
    } else {
        limit = 0xF660;
    }
    D_800C2038 = (D_800C2038 + 1) % D_800C204C[region];
    while (mode[0] * mode[1] > limit) {
        D_800C2038 = (D_800C2038 + 1) % D_800C204C[region];
    }
    D_800C2030 = MAX(D_800C2054[region][D_800C2038].width, D_800C2054[region][D_800C203C].width);
    D_800C2034 = MAX(D_800C2054[region][D_800C2038].height, D_800C2054[region][D_800C203C].height);
}
