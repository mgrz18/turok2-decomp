#include "common.h"

extern s32 func_00286E14(s32, s32, void *, s32, s32, s32, f32, f32);
extern u8 D_800B3644[];
extern s32 D_800C2030; /* screen width */

void func_002893D8(s32 arg0) {
    f32 x;

    if (D_800C2030 >= 0x11D) {
        x = D_800C2030 / 2 - 12.0f;
    } else {
        x = D_800C2030 / 2;
    }
    func_00286E14(arg0, 1, D_800B3644, x, 12, 10, 0.7f, 1.0f);
}
