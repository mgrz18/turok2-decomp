#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

s32 func_00268BF8(CGameObjectInstance *arg0) {
    return arg0->m_dwFlags & 1;
}
