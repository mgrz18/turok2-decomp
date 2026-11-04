#include "common.h"
#include "structs/game_object.h"

void func_004470E0(CGameObjectInstance *arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
    }
}
