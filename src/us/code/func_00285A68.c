#include "common.h"
#include "m2c_macros.h"

extern u8 D_800F5EA0[];

void Controller_WatchPak(s32 arg0) {
    D_800F5EA0[arg0] = 1;
}
