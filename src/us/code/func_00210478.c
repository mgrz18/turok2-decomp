#include "common.h"
#include "structs/game_object.h"

void func_00210478(CMtxF mf, f32 arg1, f32 arg2, f32 arg3) {
    mf[0][0] = (f32) (mf[0][0] * arg1);
    mf[1][0] = (f32) (mf[1][0] * arg1);
    mf[2][0] = (f32) (mf[2][0] * arg1);
    mf[3][0] = (f32) (mf[3][0] * arg1);
    mf[0][1] = (f32) (mf[0][1] * arg2);
    mf[1][1] = (f32) (mf[1][1] * arg2);
    mf[2][1] = (f32) (mf[2][1] * arg2);
    mf[3][1] = (f32) (mf[3][1] * arg2);
    mf[0][2] = (f32) (mf[0][2] * arg3);
    mf[1][2] = (f32) (mf[1][2] * arg3);
    mf[2][2] = (f32) (mf[2][2] * arg3);
    mf[3][2] = (f32) (mf[3][2] * arg3);
}
