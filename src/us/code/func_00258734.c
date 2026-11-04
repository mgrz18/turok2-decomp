#include "common.h"
#include "structs/game_object.h"

s32 func_00248720(s32, s32);

/* Weapon callback: forwards to the owning player (m_AI.m_pParent). */
void func_00258734(CGameObjectInstance *arg0, s32 arg1, s32 arg2) {
    func_00248720((s32)arg0->m_AI.m_pParent, arg2);
}
