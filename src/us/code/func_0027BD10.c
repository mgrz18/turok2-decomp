#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[0xC - 0x8];
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s8 unkE;
    /* 0x0F */ u8 padF[0x12 - 0xF];
    /* 0x12 */ s8 unk12;
} Unk0027BD10;

void func_0027BD10(Unk0027BD10 *arg0) {
    arg0->unk4 = 1;
    arg0->unk0 = 0;
    arg0->unkC = 0;
    arg0->unkE = 0;
    arg0->unk12 = -1;
}
