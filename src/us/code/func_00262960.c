#include "common.h"
#include "structs/game_object.h"

s32 CScene__GetInstanceSavedBit(s32 *, void *);               /* extern */
extern s32 D_800F7078;

void Turret_EventShow(CGameObjectInstance *arg0) {
    if (CScene__GetInstanceSavedBit(&D_800F7078, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    }
}
