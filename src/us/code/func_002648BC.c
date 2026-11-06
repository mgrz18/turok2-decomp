#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

s32 CScene__SetInstanceSavedBit(s32 *, void *, s32);  /* extern */

void Platform_EventOff(CGameObjectInstance *arg0) {
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
}
