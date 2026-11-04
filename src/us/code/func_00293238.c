#include "common.h"

/* The source record: words at 8 and 0xC, a halfword at 0x1A. */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x1A - 0x10];
    /* 0x1A */ u16 unk1A;
} CopySource;

/* What func_00293238 fills from it. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[4];
    /* 0x0C */ u16 unkC;
} CopyDest;

void func_00293238(CopySource *arg0, CopyDest *arg1) {
    arg1->unk0 = arg0->unk8;
    arg1->unkC = arg0->unk1A;
    arg1->unk4 = arg0->unkC;
}
