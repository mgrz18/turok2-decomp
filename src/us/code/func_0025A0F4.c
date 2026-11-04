#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800AFA50;

void func_0025A0F4(CGameObjectInstance *arg0, s32 arg1) {
    func_00245A98((s32) arg0, arg1, (s32) &D_800AFA50, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wPatrolAnimFlags);
}
