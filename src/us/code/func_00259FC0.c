#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);

extern u8 D_800AF900[];

void func_00259FC0(CGameObjectInstance *obj, CAI *ai) {
    func_00245A98(obj, ai, D_800AF900, obj->header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags);
}
