#include "common.h"
#include "m2c_macros.h"
#include "globals/crash.h"
#include "globals/framebuffer.h"

void Crash_ResetFrameBuffer(void) {
    sCrashFrameBuffer = gCFB[0];
}
