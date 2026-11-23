/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BEA8.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

/* Calls CPlayerPool__SetInvincibility on the players with 900, then queues MotherBoss_OnDefeatCinemaDone_2. */

extern void CPlayerPool__SetInvincibility(void *players, f32 value);
extern void MotherBoss_OnDefeatCinemaDone_2(void);
extern s32 Cinema_SetNextCallbacks(s32 id, void *arg1, void *arg2);

void MotherBoss_StartDefeatCinema_2(void) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    Cinema_SetNextCallbacks(0xE6, MotherBoss_OnDefeatCinemaDone_2, NULL);
}
