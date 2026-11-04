#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 param1C;
    /* 0x20 */ s32 param20;
} MenuItem;

s32 func_00416644(s32, s32, s32, s32, s32);
s32 func_00275904(s32);                     /* extern */
s32 func_00421170();                            /* extern */

extern s32 D_0043B5C8;
extern s32 D_80119E2C;

s32 func_004511B4(s32 arg0, MenuItem *arg1) {
    s32 temp_s1;

    temp_s1 = arg1->param1C;
    func_00421170();
    func_00275904(-1);
    func_00416644((s32) &D_80119E2C, (s32) &D_0043B5C8, temp_s1, arg1->param20, 0);
    return 1;
}
