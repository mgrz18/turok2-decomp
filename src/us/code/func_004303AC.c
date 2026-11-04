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

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x44 - 0x10];
    /* 0x44 */ s32 unk44;
} SessionState;

s32 func_0042C194(s32, s32, s32, s32, s32);
extern s32 D_800F7078;
extern SessionState D_8011ACB0;

void func_004303AC(BootState *arg0) {
    u8 sp18[0x18];
    s32 var_s1;

    var_s1 = -1;
    if (func_0042C194((s32) &D_800F7078, -1, -1, (s32) sp18, 1) == 0) {
        var_s1 = 0;
    }
    D_8011ACB0.unk0 = 0;
    D_8011ACB0.unk4 = 0;
    D_8011ACB0.unk8 = 0;
    D_8011ACB0.unkC = 0;
    arg0->unk23FFC = 0;
    D_8011ACB0.unk44 = 0;
    arg0->unk23FE1 = 2;
    arg0->unk23FF8 = var_s1;
    arg0->state = 0xB;
}
