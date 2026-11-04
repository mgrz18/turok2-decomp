#include "common.h"

s32 func_0028591C(s32);
s32 func_0041DD90(s32, s32, s32);

extern s32 D_800C2028;
extern s32 D_80130990;
extern s32 D_801309A4;

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s8 slot;
} MenuItem;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ MenuItem *item;
} MenuCaller;

s32 func_00423964(MenuCaller *arg0) {
    s32 var_a0;
    s32 var_a1;
    s32 var_v1;

    if (D_801309A4 == 0) {
        if (D_80130990 != 0) {
            var_v1 = D_800C2028;
        } else {
            var_v1 = (s32) arg0->item->slot;
        }
        if (var_v1 != -1) {
            if (D_80130990 != 0) {
                var_a0 = D_800C2028;
            } else {
                var_a0 = (s32) arg0->item->slot;
            }
            if (func_0028591C(var_a0) != 0) {
                if (D_80130990 != 0) {
                    var_a1 = D_800C2028;
                } else {
                    var_a1 = (s32) arg0->item->slot;
                }
                if (func_0041DD90((s32) arg0, var_a1, 0) != 0) {
                    D_801309A4 = 1;
                }
            }
        }
    }
    return D_801309A4;
}
