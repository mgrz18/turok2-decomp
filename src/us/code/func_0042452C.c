#include "common.h"
#include "m2c_macros.h"

extern s32 gVideoClearFrames;

void func_0042452C(void) {
    if (gVideoClearFrames == 0) {
        gVideoClearFrames = 0x14;
    }
}
