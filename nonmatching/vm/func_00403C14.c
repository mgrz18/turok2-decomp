/* NON-MATCHING: 20 of 57 words differ: in mode 1 the ROM loads 0x3200 into its own register and the 30.0f literal early (scheduling); stores and their order match. */
#include "common.h"

typedef struct {
    /* 0x000 */ s32 mode;
    /* 0x004 */ u8 pad0[0x354 - 0x4];
    /* 0x354 */ f32 unk354;
    /* 0x358 */ u8 pad1[0x380 - 0x358];
    /* 0x380 */ s32 values[2][8];
    /* 0x3C0 */ s32 unk3C0;
    /* 0x3C4 */ s32 unk3C4;
    /* 0x3C8 */ u8 unk3C8;
    /* 0x3C9 */ u8 unk3C9;
} ModeState00403C14;

extern ModeState00403C14 D_8012F608;

void func_00403C14(s32 mode) {
    if (mode == 1) {
        D_8012F608.unk3C4 = 120;
        D_8012F608.values[0][3] = 0x1400;
        D_8012F608.values[0][4] = 0x1400;
        D_8012F608.values[0][6] = 0x1400;
        D_8012F608.values[0][5] = 0x1400;
        D_8012F608.values[1][3] = 0x1400;
        D_8012F608.values[1][4] = 0x1400;
        D_8012F608.values[1][6] = 0x1400;
        D_8012F608.values[1][5] = 0x1400;
        D_8012F608.values[0][0] = 0x5000;
        D_8012F608.values[1][0] = 0x5000;
        D_8012F608.values[0][2] = 0x3200;
        D_8012F608.values[1][2] = 0x3200;
        D_8012F608.values[0][1] = 0xC800;
        D_8012F608.values[1][1] = 0xC800;
        D_8012F608.unk354 = 30.0f;
    } else if (mode == 2) {
        D_8012F608.unk3C4 = 5;
        D_8012F608.values[0][2] = 0x3200;
        D_8012F608.values[0][0] = 0x5000;
        D_8012F608.values[1][2] = 0x3200;
        D_8012F608.values[1][0] = 0x5000;
    } else if (mode == 3) {
        D_8012F608.unk3C4 = 2;
        D_8012F608.values[0][1] = 0xC800;
        D_8012F608.values[1][1] = 0xC800;
    }
    D_8012F608.mode = mode;
    D_8012F608.unk3C8 = 0;
    D_8012F608.unk3C9 = 0;
}
