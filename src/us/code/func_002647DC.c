#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

void func_002647DC(CGameObjectInstance *arg0, s32 *arg1) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    *arg1 |= 0x08000000;
    func_00243414((s32) arg0, (s32) arg1, 1);
}
