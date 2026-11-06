#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"

/* AI state: calls CPlayerPool__SetInvincibility on the players with 900, then goes to anim state 0x1F once the anim is done. */

extern void CPlayerPool__SetInvincibility(void *players, f32 value);
extern void CAI__SetMode(void *obj, CAI *ai, s32 state);

void MotherBoss_Mode30_DeathStep(void *obj, CAI *ai) {
    CPlayerPool__SetInvincibility(&D_80119870.m_PlayerList, 900.0f);
    if (ai->m_AnimRepeatedCount > 0) {
        CAI__SetMode(obj, ai, 0x1F);
    }
}
