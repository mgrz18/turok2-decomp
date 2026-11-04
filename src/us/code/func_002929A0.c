#include "common.h"

/* What gets bound: three bytes at 0..2 and a halfword at 0xC. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 pad3[0xC - 3];
    /* 0x0C */ u16 unkC;
} BindSource;

/* A 0x10-byte slot of the table at 0x60. */
typedef struct {
    /* 0x00 */ BindSource *source;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ s8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 unk9;
    /* 0x0A */ s8 unkA;
    /* 0x0B */ s8 unkB;
    /* 0x0C */ f32 unkC;
} BindSlot;

typedef struct {
    /* 0x00 */ u8 pad0[0x60];
    /* 0x60 */ BindSlot *slots;
} BindTable;

extern f32 D_800AA410;

/* Resets slot arg1 to its defaults. */
void func_002929A0(BindTable *arg0, s32 arg1) {
    arg0->slots[arg1].unk6 = 0;
    arg0->slots[arg1].unkA = 0;
    arg0->slots[arg1].unk7 = 0x40;
    arg0->slots[arg1].unk9 = 0x7F;
    arg0->slots[arg1].unk8 = 5;
    arg0->slots[arg1].unkB = 0;
    arg0->slots[arg1].unk4 = 0xC8;
    arg0->slots[arg1].unkC = D_800AA410;
}
