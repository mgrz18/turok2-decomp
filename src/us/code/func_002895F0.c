#include "common.h"

s32 CEngineApp__DrawWorld();                            /* extern */
s32 func_00289108(sptr);                         /* extern */

void EngineMode05_Cinema_Draw(sptr arg0) {
    CEngineApp__DrawWorld();
    func_00289108(arg0);
}
