#include "common.h"

/* The world (D_800F7078): the fields this resets. */
typedef struct {
    /* 0x00000 */ u8 pad0[0x18FAC];
    /* 0x18FAC */ s32 selectionState;
    /* 0x18FB0 */ s32 frozen;
    /* 0x18FB4 */ s32 unk18FB4;
    /* 0x18FB8 */ s32 unk18FB8;
    /* 0x18FBC */ u8 pad18FBC[0x18FD0 - 0x18FBC];
    /* 0x18FD0 */ s32 unk18FD0;
    /* 0x18FD4 */ s32 selection;
    /* 0x18FD8 */ s32 unk18FD8;
    /* 0x18FDC */ s32 unk18FDC;
} World;

void func_0042C4F4(World *arg0) {
    arg0->selectionState = 2;
    arg0->frozen = 0;
    arg0->unk18FB4 = 0;
    arg0->unk18FB8 = 0;
    arg0->unk18FD0 = 0;
    arg0->selection = 0;
    arg0->unk18FD8 = 0;
    arg0->unk18FDC = 0;
}
