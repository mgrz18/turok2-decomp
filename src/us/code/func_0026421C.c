#include "common.h"
#include "structs/game_object.h"

s32 func_00220408(s32, s32);
s32 func_00243414(s32, s32, s32);

extern s32 D_800F7078;

void func_0026421C(CGameObjectInstance *arg0, CAI *arg1) {
    arg1->m_Action = 0;
    func_00243414(arg0, (s32) arg1, 0);
    if (func_00220408((s32) &D_800F7078, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
    }
}
