#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

void func_00444324(CGameObjectInstance *arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags & 0xF7FFFFFF);
        func_00243414((s32) arg0, (s32) arg1, 2);
    }
}
