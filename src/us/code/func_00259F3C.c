#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800AF870;

void func_00259F3C(CGameObjectInstance *arg0, s32 arg1) {
    func_00245A98((s32) arg0, arg1, (s32) &D_800AF870, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wAlertAnimFlags);
}
