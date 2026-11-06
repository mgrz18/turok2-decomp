#include "common.h"
#include "structs/game_object.h"

s32 CScene__SetInstanceSavedBit(s32, s32, s32);

s32 GameObject_FireTriggers(void *, s32, void *);     /* extern */
extern s32 gScene;

void EnemyMode_Death_Enter(CGameObjectInstance *arg0, CAI *arg1) {
    s32 temp_a0;
    CEnemyIntelligence *sub;

    arg1->m_Health = 0;
    arg1->m_ModeLastFrame = -1;
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x20000);
    temp_a0 = arg0->header.m_pIntelligence->m_Type;
    if (((temp_a0 == 1) || (temp_a0 == 4)) && ((u32) ((u8) arg1->m_Mode - 0x35) < 7U)) {
        arg1->m_dwFlags |= 0x20;
    }
    sub = &arg0->header.m_pIntelligence->m_Enemy;
    if (!(arg0->m_dwFlags & 0x80000)) {
        if (!(sub->m_dwFlags & 0x20)) {
            CScene__SetInstanceSavedBit((s32) &gScene, (s32) arg0, 1);
        }
        GameObject_FireTriggers(arg0, 1, arg0);
    }
}
