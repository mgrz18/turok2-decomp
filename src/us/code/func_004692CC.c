#include "common.h"

/* The play-time clock (see func_00288FDC.c) and a word at 0x44. */
typedef struct {
    /* 0x00 */ f32 frames;
    /* 0x04 */ f32 seconds;
    /* 0x08 */ f32 minutes;
    /* 0x0C */ f32 hours;
    /* 0x10 */ u8 pad10[0x44 - 0x10];
    /* 0x44 */ s32 unk44;
} PlayClock;

/* The fields this sets, far into this (large) state. */
typedef struct {
    /* 0x00000 */ u8 pad0[0x23FDC];
    /* 0x23FDC */ s32 mode;
    /* 0x23FE0 */ u8 pad23FE0;
    /* 0x23FE1 */ s8 unk23FE1;
    /* 0x23FE2 */ u8 pad23FE2[0x23FF8 - 0x23FE2];
    /* 0x23FF8 */ s32 result;           /* 0, or -1 when func_0042C0B4 failed */
    /* 0x23FFC */ s32 unk23FFC;
} BigState;

s32 func_0042C0B4(void *, s32, s32, void *, s32);   /* extern */
extern u8 D_800F7078[];
extern PlayClock D_8011ACB0;

void func_004692CC(BigState *arg0) {
    u8 sp18[0x18];
    s32 var_s1;

    var_s1 = -1;
    if (func_0042C0B4(D_800F7078, -1, -1, sp18, 1) == 0) {
        var_s1 = 0;
    }
    D_8011ACB0.frames = 0;
    D_8011ACB0.seconds = 0;
    D_8011ACB0.minutes = 0;
    D_8011ACB0.hours = 0;
    arg0->unk23FFC = 0;
    D_8011ACB0.unk44 = 0;
    arg0->unk23FE1 = 2;
    arg0->result = var_s1;
    arg0->mode = 0xB;
}
