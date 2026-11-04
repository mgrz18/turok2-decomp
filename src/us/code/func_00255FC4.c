#include "common.h"

s32 func_00283FF8(s32, s32, s32 *); /* extern */
extern s32 D_800F1FD8;
extern s32 func_002577DC;

void func_00255FC4(s32 arg0) {
    D_800F1FD8 = arg0;
    func_00283FF8(0x50, 0, &func_002577DC);
}
