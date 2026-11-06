#include "common.h"
#include "m2c_macros.h"

extern s32 D_80125C48;

s32 adecGetStopFlag(void) {
    return D_80125C48;
}
