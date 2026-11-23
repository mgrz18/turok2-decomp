#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"


/* An event handler: (instance, AI, event value), the last two unused. */
void Turret_EventShow(CGameObjectInstance *arg0, CAI *ai, s32 arg2) {
    if (CScene__GetInstanceSavedBit(&gScene, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    }
}
