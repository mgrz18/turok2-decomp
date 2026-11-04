#include "common.h"

typedef struct {
    /* 0x00000 */ u8 pad0[0x23FDC];
    /* 0x23FDC */ s32 state;
    /* 0x23FE0 */ s8 unk23FE0;
    /* 0x23FE1 */ s8 unk23FE1;
    /* 0x23FE2 */ u8 pad23FE2[2];
    /* 0x23FE4 */ s32 unk23FE4;
    /* 0x23FE8 */ s32 ready;
    /* 0x23FEC */ u8 pad23FEC[0x23FF8 - 0x23FEC];
    /* 0x23FF8 */ s32 unk23FF8;
    /* 0x23FFC */ s32 unk23FFC;
} BootState;

void func_0028891C(BootState *arg0) {
    arg0->unk23FE1 = 1;
    arg0->unk23FE4 = 0;
    arg0->unk23FE0 = 0;
}
