#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

void func_00444370(CGameObjectInstance *arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags & 0xEFFFFFFF);
        func_00243414((s32) arg0, (s32) ai, 2);
    }
}
