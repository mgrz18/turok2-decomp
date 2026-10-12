#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

void func_00447430(CGameObjectInstance *arg0, void *arg1) {
    if (M2C_FIELD(arg1, s8 *, 0xC7) != 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
    }
}
