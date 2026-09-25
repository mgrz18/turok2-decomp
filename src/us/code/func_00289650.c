#include "common.h"

extern s32 func_00288D3C(s32, f32, s32, s32);
extern s32 func_0042F57C(void);

void func_00289650(s32 arg0) {
    func_0042F57C();
    func_00288D3C(arg0, 20.0f, 0xD, 8);
}
