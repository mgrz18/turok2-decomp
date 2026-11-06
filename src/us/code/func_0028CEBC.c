#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011F180;
extern s32 D_80130A88;

void Crash_ResetFrameBuffer(void) {
    D_8011F180 = D_80130A88;
}
