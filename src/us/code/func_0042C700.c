#include "common.h"

/* The world (D_800F7078): the fields this sets. */
typedef struct {
    /* 0x00000 */ u8 pad0[0x18FAC];
    /* 0x18FAC */ s32 selectionState;
    /* 0x18FB0 */ s32 frozen;
    /* 0x18FB4 */ u8 pad18FB4[0x18FD4 - 0x18FB4];
    /* 0x18FD4 */ s32 selection;
} World;

s32 func_00275624(s32);

void func_0042C700(World *arg0, s32 arg1, s32 arg2) {
    arg0->frozen = 1;
    arg0->selectionState = arg2;
    arg0->selection = arg1;
    if ((arg2 == 2) && (arg1 != 0x3E7)) {
        func_00275624(0xBE0);
    }
}
