#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/rumble.h"

s32 CPlayerPool__StopRumble(CPlayerPool *);                   /* extern */
s32 RumblePool_FreeAll(RumblePool *pool);                   /* extern */

void CEngineApp__StopRumble(void) {
    RumblePool_FreeAll(&gRumblePool);
    CPlayerPool__StopRumble(&gEngineApp_m_Game.m_PlayerList);
}
