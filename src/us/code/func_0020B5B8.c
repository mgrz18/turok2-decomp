#include "common.h"
#include "m2c_macros.h"

extern s32 gCurrentCombineMode;
extern s32 gCurrentRenderMode;

void Render_InvalidateModeCache(void) {
    gCurrentRenderMode = -1;
    gCurrentCombineMode = -1;
}
