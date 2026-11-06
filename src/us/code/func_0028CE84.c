#include "common.h"
#include "m2c_macros.h"

extern s16 sCrashBgColor;

void Crash_SetBackgroundColor(s32 arg0, s32 arg1, s32 arg2) {
    sCrashBgColor = ((arg0 << 8) & ~0x7FF) | ((arg1 * 8) & 0x7C0) | ((arg2 >> 2) & 0x3E) | 1;
}
