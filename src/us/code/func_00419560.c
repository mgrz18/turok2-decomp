#include "common.h"

s32 func_00426480(s32);

s32 func_00285A68(s8);                          /* extern */

/* arg0->0x20 points at the current selection record; byte 4 is its index. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s8 index;
    /* 0x05 */ u8 pad5[0xB0 - 0x5];
    /* 0xB0 */ s32 flags;
} Selection;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ Selection *sel;
} SelectionOwner;

void func_00419560(SelectionOwner *arg0) {
    s8 temp_s0;

    temp_s0 = arg0->sel->index;
    func_00285A68(temp_s0);
    func_00426480((s32) temp_s0);
}
