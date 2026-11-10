#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"

s32 Cinema_RequestForObject(sptr, sptr);


void InteractiveAnim_EventPlayCinema(CGameObjectInstance *arg0, s32 arg1, s32 arg2) {
    Cinema_RequestForObject(arg0, arg2);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
}
