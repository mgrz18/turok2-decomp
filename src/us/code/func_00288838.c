#include "common.h"
#include "structs/game.h"

s32 CEngineApp__Idle(s32 *, s32);              /* extern */

void Engine_Idle(s32 arg0) {
    CEngineApp__Idle(&gEngineApp, arg0);
}
