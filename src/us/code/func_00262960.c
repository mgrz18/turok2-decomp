#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

s32 CScene__GetInstanceSavedBit(s32 *, void *);               /* extern */

void Turret_EventShow(CGameObjectInstance *arg0) {
    if (CScene__GetInstanceSavedBit(&gScene, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    }
}
