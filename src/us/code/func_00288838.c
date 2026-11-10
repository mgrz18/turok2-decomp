#include "common.h"
#include "structs/game.h"

s32 CEngineApp__Idle(s32 *, sptr);              /* extern */

void Engine_Idle(sptr arg0) {
    CEngineApp__Idle(&gEngineApp, arg0);
}
