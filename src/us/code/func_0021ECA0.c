#include "common.h"
#include "globals/engine.h"
#include "structs/game_object.h"

extern void CGameObjectInstance__DrawModel(CGameObjectInstance *arg0, sptr arg1, sptr arg2);

void CGameObjectInstance__DrawModelWithLights(CGameObjectInstance *arg0, s32 arg1, s32 arg2, CLights1 *src) {
    arg0->m_Lights[gEvenOdd] = *src;
    CGameObjectInstance__DrawModel(arg0, arg1, arg2);
}
