#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"

s32 CPlayer__StartWeaponBackgroundSound(void *, s32);             /* extern */
s32 Sound__StopSoundId(s32);                     /* extern */
extern s32 D_800B6D20;

void WeaponSound_Update_AF12C(CGameObjectInstance *arg0, CAI *arg1) {
    CPlayer *temp_a0;

    temp_a0 = (CPlayer *) arg0->m_AI.m_pParent;
    if (temp_a0->m_pLoopingSound_WeaponBackground == 0) {
        if (arg1->m_Mode != 1) {
            CPlayer__StartWeaponBackgroundSound(temp_a0, 0x1A8);
        }
    }
    if ((arg1->m_Mode != 4) && (D_800B6D20 & 1)) {
        Sound__StopSoundId(0x1A9);
    }
}
