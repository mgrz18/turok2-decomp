#include "common.h"
#include "m2c_macros.h"
#include "globals/boss.h"
#include "protos/ai.h"



void MantidQueen_UpdateMode2(s32 arg0, s32 arg1) {
    if (gMantidQueen.waveActive == 0) {
        CAI__SetMode(arg0, arg1, 3);
    }
}
