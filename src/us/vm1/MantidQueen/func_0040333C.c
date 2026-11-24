#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

/* Turok 2 differs from Rage Wars here: no trigger fields are read; it calls
 * CPlayerPool__SetInvincibility on the player list with 900.0f, then Cinema_RequestById(0x136). */
void CPlayerPool__SetInvincibility(CPlayerPool *, f32);
s32 Cinema_RequestById(s32);

void MantidQueen_EnterDeath(void *arg0) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    Cinema_RequestById(0x136);
}
