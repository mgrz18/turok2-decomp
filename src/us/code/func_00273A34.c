#include "common.h"

/* A sound channel (see func_00274850.c): the fields reset here. */
typedef struct {
    /* 0x00 */ s32 index;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[0x2C - 0x18];
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ u8 pad30[4];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad44[0x58 - 0x44];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 pad60[0xA4 - 0x60];
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ u8 padA8[4];
    /* 0xAC */ s32 unkAC;
    /* 0xB0 */ s32 bank;
    /* 0xB4 */ s32 unkB4;
    /* 0xB8 */ f32 unkB8;
    /* 0xBC */ s32 unkBC;
    /* 0xC0 */ s32 unkC0;
    /* 0xC4 */ s32 unkC4;
} Channel;

void func_00273A34(Channel *arg0, s32 arg1, s32 arg2) {
    arg0->unkC = -1;
    arg0->unk8 = -1;
    arg0->unk3A = -1;
    arg0->unk40 = -1;
    arg0->unkB4 = -1;
    arg0->bank = arg1;
    arg0->index = arg2;
    arg0->unk38 = 0;
    arg0->unk14 = 0;
    arg0->unk58 = 0;
    arg0->unk5C = 0;
    arg0->unkA4 = 0;
    arg0->unkAC = 0;
    arg0->unkBC = 0;
    arg0->unkC0 = 0;
    arg0->unkC4 = 1;
    arg0->unk2C = 1.0f;
    arg0->unk34 = 1.0f;
    arg0->unkB8 = 1.0f;
}
