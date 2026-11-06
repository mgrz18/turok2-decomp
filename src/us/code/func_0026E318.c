#include "common.h"
#include "structs/game_object.h"

void CGameObjectInstance__AdvanceAnimation();                               /* extern */

void Event_Show(CGameObjectInstance *arg0) {
    CGameObjectInstance__AdvanceAnimation();
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
}
