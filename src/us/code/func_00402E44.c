#include "common.h"
#include "m2c_macros.h"

s32 CAI__SetMode(s32, s32, s32);

extern s32 gMantidQueen_waveActive;

void MantidQueen_UpdateMode5(s32 arg0, s32 arg1) {
    if (gMantidQueen_waveActive == 0) {
        CAI__SetMode(arg0, arg1, 6);
    }
}
