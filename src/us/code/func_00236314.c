#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[4];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
} Arg0;

void func_00236314(Arg0 *arg0, s32 arg1) {
    arg0->unk4 = 0;
    arg0->unkC = arg1;
    arg0->unk10 = arg1;
}
