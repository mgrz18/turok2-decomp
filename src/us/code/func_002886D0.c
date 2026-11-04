#include "common.h"

/* The multiplayer match state at D_8011ACB0: the fields this resets. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x44 - 0x10];
    /* 0x44 */ s32 unk44;
} MatchState;

/* The game/menu context this runs on. */
typedef struct {
    /* 0x00000 */ u8 pad0[0x23FDC];
    /* 0x23FDC */ s32 unk23FDC;
    /* 0x23FE0 */ u8 pad23FE0;
    /* 0x23FE1 */ s8 unk23FE1;
    /* 0x23FE2 */ u8 pad23FE2[0x23FF8 - 0x23FE2];
    /* 0x23FF8 */ s32 unk23FF8;
    /* 0x23FFC */ s32 unk23FFC;
} Context;

s32 func_0042C194(s32, s32, s32, s32, s32);
extern s32 D_800F7078;
extern MatchState D_8011ACB0;

void func_002886D0(Context *arg0, s32 arg1) {
    u8 sp18[0x18];
    s32 var_s0;

    var_s0 = arg1;
    if (func_0042C194((s32) &D_800F7078, -1, var_s0, (s32) sp18, 1) == 0) {
        var_s0 = 0;
    }
    D_8011ACB0.unk0 = 0;
    D_8011ACB0.unk4 = 0;
    D_8011ACB0.unk8 = 0;
    D_8011ACB0.unkC = 0;
    D_8011ACB0.unk44 = 0;
    arg0->unk23FE1 = 2;
    arg0->unk23FF8 = var_s0;
    arg0->unk23FFC = 1;
    arg0->unk23FDC = 0xD;
}
