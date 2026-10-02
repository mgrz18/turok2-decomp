#include "common.h"
#include "m2c_macros.h"

s32 func_00416894(s32, s32, s32);

extern M2C_UNK D_004389CC;

s32 func_0045242C(s32 arg0, s32 arg1, s32 arg2) {
    func_00416894(arg2, arg1, (s32) &D_004389CC);
    return 1;
}
