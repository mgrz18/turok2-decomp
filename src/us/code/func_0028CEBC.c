#include "common.h"
#include "m2c_macros.h"
#include "globals/crash.h"

extern s32 gCFB;

void Crash_ResetFrameBuffer(void) {
    sCrashFrameBuffer = (u16 *) gCFB;
}
