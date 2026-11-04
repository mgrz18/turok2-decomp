#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800AF750;

void func_00259EB8(CGameObjectInstance *obj, s32 arg1) {
    func_00245A98((s32) obj, arg1, (s32) &D_800AF750, obj->header.m_pIntelligence->m_Enemy.m_wIdleAnimFlags);
}
