#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/boss.h"
#include "globals/game.h"
#include "globals/boss.h"
#include "protos/ai.h"
#include "protos/player.h"


s32 CPlayer__Warp(sptr, s32, s32, s32);


void MotherBoss_OnDefeatCinemaDone(void) {
    CPlayer *temp_v0;
    GameSettings *temp_v1;

    CAI__SetMode(gMotherBoss.obj, &gMotherBoss.obj->m_AI, 1);
    temp_v0 = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (temp_v0 != NULL) {
        temp_v1 = &gEngineApp_m_Game.m_Settings;
        temp_v0->m_Data.m_bMotherBossComplete = 1;
        temp_v1->unlockedCheats = (s32) (temp_v1->unlockedCheats | 4);
        CPlayer__Warp((sptr) temp_v0, 0x1B57, 2, 0);
    }
}
