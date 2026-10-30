/* NON-MATCHING: 80 of 135 words differ: the mode-cycling loop and the two maxima are scheduled differently */
/* WRONG CUT: one function 0x004242EC-0x00424508 (size 0x21C); splat cut its epilogue
 * (jr ra; addiu sp) into func_00424500. */
#include "common.h"

s32 func_00266B80(void); /* osGetMemSize */

#define MAX(a, b) ((a) < (b) ? (b) : (a))

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
extern s32 D_800C204C[2]; /* mode count per region */
extern VideoMode D_800C2054[2][5];

void func_004242EC(void) {
    s32 region = 0;
    VideoMode *mode;
    s32 limit;

    switch (D_80000300) {
        case 0:
        case 2:
            region = 1;
            break;
        case 1:
            break;
    }
    mode = &D_800C2054[region][D_800C2038];
    limit = (func_00266B80() == 0x400000) ? 0xF660 : 0x2A300;
    D_800C2038 = (D_800C2038 + 1) % D_800C204C[region];
    while (mode->width * mode->height > limit) {
        D_800C2038 = (D_800C2038 + 1) % D_800C204C[region];
    }
    D_800C2030 = MAX(D_800C2054[region][D_800C2038].width, D_800C2054[region][D_800C203C].width);
    D_800C2034 = MAX(D_800C2054[region][D_800C2038].height, D_800C2054[region][D_800C203C].height);
}
