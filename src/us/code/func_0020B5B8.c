#include "common.h"
#include "m2c_macros.h"
#include "globals/render.h"


void Render_InvalidateModeCache(void) {
    gCurrentRenderMode = -1;
    gCurrentCombineMode = -1;
}
