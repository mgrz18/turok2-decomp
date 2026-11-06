#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

/* Calls CPlayerPool__SetInvincibility on the players with 900, then queues MotherBoss_OnDefeatCinemaDone. */

extern void CPlayerPool__SetInvincibility(void *players, f32 value);
extern void MotherBoss_OnDefeatCinemaDone(void);
extern s32 Cinema_SetNextCallbacks(s32 id, void *arg1, void *arg2);

void MotherBoss_StartDefeatCinema(void) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    Cinema_SetNextCallbacks(0xE6, MotherBoss_OnDefeatCinemaDone, NULL);
}
