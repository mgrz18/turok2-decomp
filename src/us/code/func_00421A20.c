#include "common.h"
#include "m2c_macros.h"

void func_00285A80(s32);
s32 func_0042C700(s32, s32, s32);

extern M2C_UNK D_800F7078;
extern s32 D_80110054;

void func_00421A20(s32 arg0, s32 arg1) {
    func_00285A80(arg1);
    func_0042C700((s32) &D_800F7078, D_80110054, 2);
}
