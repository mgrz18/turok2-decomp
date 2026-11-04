#include "common.h"
#include "structs/game_object.h"

s32 func_00253770(s32, void *, s32, s32); /* extern */

void func_0025689C(CGameObjectInstance *arg0, s32 arg1, s32 arg2) {
    func_00253770((s32) arg0->m_AI.m_pParent, arg0, arg1, arg2);
}
