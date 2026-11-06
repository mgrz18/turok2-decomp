#include "common.h"
#include "m2c_macros.h"

extern s32 sCrashFrameBuffer;
extern s32 gCFB;

void Crash_ResetFrameBuffer(void) {
    sCrashFrameBuffer = gCFB;
}
