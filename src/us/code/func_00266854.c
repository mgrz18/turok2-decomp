#include "common.h"
#include "structs/game_object.h"

s32 CGameObjectInstance__AdvanceAnimation(CGameObjectInstance *);
s32 CGameObjectInstance__SetAnimByAction(CGameObjectInstance *, s32, s32);
s32 AI_PickAnimFromTable(CGameObjectInstance *, CAI *, s32, s32);

extern u8 sEnemyAnimIdle_AF6F0[];

void InteractiveAnim_Update(CGameObjectInstance *obj, CAI *ai) {
    s32 temp_a1;

    if (((u32) ((u16) ai->m_AnimType - 0xFA0) < 0x401U) && (ai->m_AnimRepeatedCount != 0)) {
        temp_a1 = AI_PickAnimFromTable(obj, ai, (s32) sEnemyAnimIdle_AF6F0, -1);
        ai->m_ModeLastFrame = -1;
        ai->m_AnimRepeatedCount = 0;
        ai->m_VarBuffer[0] = temp_a1;
        CGameObjectInstance__SetAnimByAction(obj, temp_a1, 1);
        CGameObjectInstance__AdvanceAnimation(obj);
    }
}
