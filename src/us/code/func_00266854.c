#include "common.h"
#include "structs/game_object.h"

s32 func_0021A7EC(CGameObjectInstance *);
s32 func_0021E9A4(CGameObjectInstance *, s32, s32);
s32 func_00245A98(CGameObjectInstance *, CAI *, s32, s32);

extern u8 D_800AF6F0[];

void func_00266854(CGameObjectInstance *obj, CAI *ai) {
    s32 temp_a1;

    if (((u32) ((u16) ai->m_AnimType - 0xFA0) < 0x401U) && (ai->m_AnimRepeatedCount != 0)) {
        temp_a1 = func_00245A98(obj, ai, (s32) D_800AF6F0, -1);
        ai->m_ModeLastFrame = -1;
        ai->m_AnimRepeatedCount = 0;
        ai->m_VarBuffer[0] = temp_a1;
        func_0021E9A4(obj, temp_a1, 1);
        func_0021A7EC(obj);
    }
}
