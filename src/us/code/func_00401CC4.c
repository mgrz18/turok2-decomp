#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x20 - 0xC];
    /* 0x20 */ s32 unk20[6];
} Module;

void func_00401CC4(Module *arg0) {
    arg0->unk8 = 0;
    arg0->unk4 = 0;
    arg0->unk20[0] = 0;
    arg0->unk20[1] = 0;
    arg0->unk20[2] = 0;
    arg0->unk20[3] = 0;
    arg0->unk20[4] = 0;
    arg0->unk20[5] = 0;
}
