#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

s32 CPlayer__Warp(s32, s32, s32, s32);

void *CPlayerPool__GetFirstActive(CPlayerPool *);                     /* extern */
s32 Cinema_IsIdle();                                /* extern */

void Primagen_UpdateDeath_2(void) {
    CPlayer *temp_v0;

    if (Cinema_IsIdle() != 0) {
        temp_v0 = CPlayerPool__GetFirstActive(&D_80119870.m_PlayerList);
        if (temp_v0 != NULL) {
            temp_v0->m_Data.m_bPrimagenBossComplete = 1;
            CPlayer__Warp((s32) temp_v0, 0x1F40, 2, 0);
        }
    }
}
