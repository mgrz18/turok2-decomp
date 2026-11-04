#include "common.h"

s32 func_00416644(s32, s32, s32, s32, s32);

s32 func_00285A68(s8);                          /* extern */
extern s32 D_0043BC64;
extern s32 D_80119E2C;

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

s32 func_00452520(s32 arg0, SelectionOwner *arg1) {
    func_00285A68(arg1->sel->index);
    func_00416644((s32) &D_80119E2C, (s32) &D_0043BC64, arg1->unk1C, (s32) arg1->sel, 0);
    return 1;
}
