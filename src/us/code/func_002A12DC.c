#include "common.h"
#include "m2c_macros.h"

extern s32 sAdecStopFlag;

void adecClearStopFlag(void) {
    sAdecStopFlag = 0;
}
