#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"


void Platform_EventOn(CGameObjectInstance *arg0) {
    CScene__SetInstanceSavedBit(&gScene, arg0, 1);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x100);
}
