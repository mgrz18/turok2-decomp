#include "common.h"

s32 CEngineApp__DrawWorld();                            /* extern */
extern s32 D_800AB8A0;

void EngineMode04_Cinema_Draw(void) {
    D_800AB8A0 = 0;
    CEngineApp__DrawWorld();
}
