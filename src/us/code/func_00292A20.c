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

/* Binds arg1 to slot arg2, copying a few of its values into the slot. */
void func_00292A20(BindTable *arg0, BindSource *arg1, s32 arg2) {
    arg0->slots[arg2].source = arg1;
    arg0->slots[arg2].unk7 = arg1->unk1;
    arg0->slots[arg2].unk9 = arg1->unk0;
    arg0->slots[arg2].unk8 = arg1->unk2;
    arg0->slots[arg2].unk4 = arg1->unkC;
}
