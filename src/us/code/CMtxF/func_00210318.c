#include "common.h"
#include "structs/game_object.h"

void CMtxF__Translate(CMtxF mf, f32 arg1, f32 arg2, f32 arg3) {
    mf[3][0] = (f32) (mf[3][0] + arg1);
    mf[3][1] = (f32) (mf[3][1] + arg2);
    mf[3][2] = (f32) (mf[3][2] + arg3);
}
