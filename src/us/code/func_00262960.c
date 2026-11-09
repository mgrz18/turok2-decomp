#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"


void Turret_EventShow(CGameObjectInstance *arg0) {
    if (CScene__GetInstanceSavedBit(&gScene, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    }
}
