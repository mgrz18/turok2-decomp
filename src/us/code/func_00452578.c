#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
} MenuCaller;

s32 func_00416644(s32, s32, s32, s32, s32);
s32 func_00421170();                            /* extern */
extern s32 D_0043B610;
extern s32 D_80119E2C;

s32 func_00452578(s32 arg0, MenuCaller *arg1) {
    func_00421170();
    func_00416644((s32) &D_80119E2C, (s32) &D_0043B610, arg1->unk1C, arg1->unk20, 0);
    return 1;
}
