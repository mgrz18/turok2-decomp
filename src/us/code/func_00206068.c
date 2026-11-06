#include "common.h"
#include "m2c_macros.h"

extern s32 D_800D8DEC;

void CCache__SetFrameDelay(s32 arg0) {
    D_800D8DEC = arg0;
}
