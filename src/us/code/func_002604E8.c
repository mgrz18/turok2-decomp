#include "common.h"
#include "structs/game_object.h"

extern s32 CScene__SetInstanceSavedBit(s32, s32, s32);
extern void GameObject_FireTriggers(void *, s32, void *);
extern u8 D_800F7078[];

void EnemyAI_SaveDeadAndFireTriggers(CGameObjectInstance *arg0) {
    CIntelligence *sub = arg0->header.m_pIntelligence;

    if (!(arg0->m_dwFlags & 0x80000)) {
        if (!(sub->m_dwTypeFlags & 0x20)) {
            CScene__SetInstanceSavedBit((s32) D_800F7078, (s32) arg0, 1);
        }
        GameObject_FireTriggers(arg0, 1, arg0);
    }
}
