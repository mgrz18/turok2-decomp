#include "common.h"
#include "m2c_macros.h"

extern s32 sAdecStopFlag;

s32 adecGetStopFlag(void) {
    return sAdecStopFlag;
}
