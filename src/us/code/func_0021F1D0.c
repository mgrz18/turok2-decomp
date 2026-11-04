#include "common.h"
#include "structs/game_object.h"

void func_0021F1D0(CGameObjectInstance *arg0, s32 arg1) {
    if (arg1 != 0) {
        arg0->m_dwHotPointFlags = (arg0->m_dwHotPointFlags | 2);
        return;
    }
    arg0->m_dwHotPointFlags = (arg0->m_dwHotPointFlags & ~2);
}
