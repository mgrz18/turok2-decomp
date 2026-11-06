#include "common.h"
#include "structs/game_object.h"

s32 CScene__SetInstanceSavedBit(s32 *, void *, s32);  /* extern */
extern s32 D_800F7078;

void Platform_EventOff(CGameObjectInstance *arg0) {
    CScene__SetInstanceSavedBit(&D_800F7078, arg0, 0);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
}
