#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/rumble.h"

s32 func_0025359C(CPlayerPool *);                   /* extern */
s32 RumblePool_FreeAll(RumblePool *pool);                   /* extern */

void func_00289FE4(void) {
    RumblePool_FreeAll(&gRumblePool);
    func_0025359C(&gEngineApp_m_Game.m_PlayerList);
}
