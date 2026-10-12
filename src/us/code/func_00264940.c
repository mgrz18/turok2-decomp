#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

void func_00264940(CGameObjectInstance *arg0, s32 *arg1) {
    *arg1 |= 0x08000000;
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
}
