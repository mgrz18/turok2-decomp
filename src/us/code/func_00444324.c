#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

void func_00444324(CGameObjectInstance *arg0, void *arg1) {
    if (M2C_FIELD(arg1, s8 *, 0xC7) != 0) {
        arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags & 0xF7FFFFFF);
        func_00243414((s32) arg0, (s32) arg1, 2);
    }
}
