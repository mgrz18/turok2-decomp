#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/boss.h"


s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
s32 CPlayer__Warp(s32, s32, s32, s32);

void *CPlayerPool__GetFirstActive(s32 *);                     /* extern */

void MotherBoss_OnDefeatCinemaDone_2(void) {
    CPlayer *temp_v0;
    GameSettings *temp_v1;

    CAI__SetMode(D_8012FB70.obj, &D_8012FB70.obj->m_AI, 1);
    temp_v0 = CPlayerPool__GetFirstActive(&D_80119870.m_PlayerList);
    if (temp_v0 != NULL) {
        temp_v1 = &D_80119870.m_Settings;
        temp_v0->m_Data.m_bMotherBossComplete = 1;
        temp_v1->unlockedCheats |= 4;
        CPlayer__Warp((s32) temp_v0, 0x1B57, 2, 0);
    }
}
