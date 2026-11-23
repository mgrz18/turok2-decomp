#include "common.h"
#include "structs/game.h"

s32 CEngineApp__Boot(s32 *);                   /* extern */

void Engine_Boot(void) {
    CEngineApp__Boot(&gEngineApp);
}
