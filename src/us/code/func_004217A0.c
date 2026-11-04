#include "common.h"

s32 func_0028591C(s32);
s32 func_0041DD90(s32, s32, s32);

extern s32 D_800C2028;
extern s32 D_80130990;
extern s32 D_801309BC;

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

s32 func_004217A0(SelectionOwner *arg0) {
    s32 var_a0;
    s32 var_a1;
    s32 var_v1;

    if (D_801309BC == 0) {
        if (D_80130990 != 0) {
            var_v1 = D_800C2028;
        } else {
            var_v1 = (s32) arg0->sel->index;
        }
        if (var_v1 != -1) {
            if (D_80130990 != 0) {
                var_a0 = D_800C2028;
            } else {
                var_a0 = (s32) arg0->sel->index;
            }
            if (func_0028591C(var_a0) != 0) {
                if (D_80130990 != 0) {
                    var_a1 = D_800C2028;
                } else {
                    var_a1 = (s32) arg0->sel->index;
                }
                if (func_0041DD90((s32) arg0, var_a1, 0) != 0) {
                    D_801309BC = 1;
                }
            }
        }
    }
    return D_801309BC;
}
