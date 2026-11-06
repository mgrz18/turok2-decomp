#include "common.h"

s32 CEngineApp__DrawWorld();                            /* extern */
s32 func_00289108(s32);                         /* extern */

void EngineMode05_Cinema_Draw(s32 arg0) {
    CEngineApp__DrawWorld();
    func_00289108(arg0);
}
