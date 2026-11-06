#include "common.h"
#include "globals/render.h"

s32 CEngineApp__DrawWorld();                            /* extern */

void EngineMode04_Cinema_Draw(void) {
    gFogEnabled = 0;
    CEngineApp__DrawWorld();
}
