#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

void func_00444310(CGameObjectInstance *arg0) {
    arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags | 0x08000000);
}
