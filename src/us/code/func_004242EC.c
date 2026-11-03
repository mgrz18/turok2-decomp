/* the real function is 0x004242EC-0x00424508 (size 0x21C, 135 words); splat cut its last two words (jr ra; addiu sp) off as func_00424500.
 * All 135 words match (checked over the real extent). First-module copy of src/us/code/func_0045D20C.c. */
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
void func_004242EC(void) {
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
