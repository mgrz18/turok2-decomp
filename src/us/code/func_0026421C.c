#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

s32 func_00220408(s32, s32);
s32 func_00243414(s32, s32, s32);

extern M2C_UNK D_800F7078;

void func_0026421C(CGameObjectInstance *arg0, void *arg1) {
    M2C_FIELD(arg1, s8 *, 0x37) = 0;
    func_00243414(arg0, (s32) arg1, 0);
    if (func_00220408((s32) &D_800F7078, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
    }
}
