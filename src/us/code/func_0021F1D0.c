#include "common.h"
#include "structs/game_object.h"

void CGameObjectInstance__SetHotPointFlag2(CGameObjectInstance *arg0, s32 arg1) {
    if (arg1 != 0) {
        arg0->m_dwHotPointFlags = (arg0->m_dwHotPointFlags | 2);
        return;
    }
    arg0->m_dwHotPointFlags = (arg0->m_dwHotPointFlags & ~2);
}
