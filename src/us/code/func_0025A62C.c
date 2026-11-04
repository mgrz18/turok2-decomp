#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);

extern u8 D_800AFEB8[];

void func_0025A62C(CGameObjectInstance *obj, CAI *ai) {
    func_00245A98(obj, ai, D_800AFEB8, obj->header.m_pIntelligence->m_Enemy.m_wLeapAnimFlags);
}
