#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800B0360;

void func_0025AA4C(CGameObjectInstance *obj, s32 arg1) {
    func_00245A98((s32) obj, arg1, (s32) &D_800B0360, obj->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}
