#include "common.h"
#include "structs/player.h"
#include "globals/game.h"

s32 CPlayer__Hurt(s32, s32, s32, s32, s32);
s32 CPlayer__UpdateOxygen(s32, s32);


void CPlayer__UpdateSubmerged(CPlayer *arg0, CGameObjectInstance *arg1) {
    s32 var_v0;

    if (gEngineApp_m_Settings.cheatFlags & 0x10) {
        var_v0 = 1;
    } else {
        var_v0 = arg0->m_Data.m_SpecialPickupCount[380] & 0xF;
    }
    if (var_v0 != 0) {
        if (arg1->m_AI.m_dwFlags & 0x80) {
            CPlayer__UpdateOxygen((s32) arg0, (s32) arg1);
        }
    } else {
        CPlayer__Hurt((s32) arg0, (s32) arg1, 0xA00, 0x02000000, 0x1C);
    }
}
