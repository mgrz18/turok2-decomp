#include "common.h"
#include "structs/game_object.h"

s32 CScene__SetInstanceSavedBit(s32, s32, s32);
s32 Cinema_RequestForObject(s32, s32);

extern s32 D_800F7078;

void InteractiveAnim_EventPlayCinema(CGameObjectInstance *arg0, s32 arg1, s32 arg2) {
    Cinema_RequestForObject(arg0, arg2);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    CScene__SetInstanceSavedBit((s32) &D_800F7078, arg0, 0);
}
