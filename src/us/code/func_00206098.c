#include "common.h"
#include "m2c_macros.h"

extern s32 D_800D8DAC;

s32 CCache__GetFreeMemEntryCount(void) {
    return D_800D8DAC;
}
