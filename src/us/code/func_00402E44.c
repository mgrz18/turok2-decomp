#include "common.h"
#include "m2c_macros.h"
#include "globals/boss.h"
#include "protos/ai.h"



void MantidQueen_UpdateMode5(sptr arg0, sptr arg1) {
    if (gMantidQueen.waveActive == 0) {
        CAI__SetMode(arg0, arg1, 6);
    }
}
