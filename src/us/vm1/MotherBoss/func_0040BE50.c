#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/ai.h"

/* AI state: calls CPlayerPool__SetInvincibility on the players with 900, then goes to anim state 0x1F once the anim is done. */

extern void CPlayerPool__SetInvincibility(void *players, f32 value);

void MotherBoss_Mode30_DeathStep(void *obj, CAI *ai) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    if (ai->m_AnimRepeatedCount > 0) {
        CAI__SetMode(obj, ai, 0x1F);
    }
}
