#include "common.h"
#include "m2c_macros.h"
#include "globals/render.h"


void Video_StartResolutionChange(void) {
    if (gVideoClearFrames == 0) {
        gVideoClearFrames = 0x14;
    }
}
