#include "common.h"

s32 func_00220260(s32, s32, s32);
s32 func_00243414(s32, s32, s32);

extern s32 D_800F7078;

void func_00265E5C(s32 arg0, s32 arg1) {
    func_00243414(arg0, arg1, 1);
    func_00220260((s32) &D_800F7078, arg0, 1);
}
