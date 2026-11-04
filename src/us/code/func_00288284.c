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

s32 func_0029B060(s32, s32, s32, s32, s32, s32);
s32 func_0029B9B0(s32, s32);
s32 func_0029BB10(s32);

s32 func_00266C5C(s32 *, s32, s32); /* extern */
extern s32 D_800B6E70;
extern s32 D_800B6E84;
extern s32 D_800C7C80;
extern s32 D_800CFC80;
extern s32 D_800F6A70;
extern s32 mainproc;

void func_00288284(BootState *arg0, s32 arg1) {
    func_00266C5C(&D_800C7C80, 1, 0x8000);
    func_0029B060((s32) &D_800F6A70, 1, (s32) &mainproc, arg1, (s32) &D_800CFC80, D_800B6E84);
    func_0029BB10((s32) &D_800F6A70);
    {
        s32 v = D_800B6E70;

        arg0->ready = 0;
        func_0029B9B0(0, v);
    }
loop_1:
    goto loop_1;
}
