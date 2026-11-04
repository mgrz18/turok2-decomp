#include "common.h"

typedef struct {
    /* 0x00 */ f32 scale;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[0x2C - 0x8];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 pad38[4];
    /* 0x3C */ s32 unk3C;
} Unk0023C0FC;

void func_0023C0FC(Unk0023C0FC *arg0) {
    arg0->unk4 = 0;
    arg0->unk2C = 0;
    arg0->unk30 = 0;
    arg0->unk34 = 0;
    arg0->unk3C = 0;
    arg0->scale = 1.0f;
}
