#include "common.h"

s32 CEngineApp__DrawWorld();                            /* extern */
extern s32 gFogEnabled;

void EngineMode04_Cinema_Draw(void) {
    gFogEnabled = 0;
    CEngineApp__DrawWorld();
}
