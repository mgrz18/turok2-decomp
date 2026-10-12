#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

s32 func_00267F34(CGameObjectInstance *arg0, void *arg1) {
    s32 var_v1;

    var_v1 = 0;
    if ((arg0->m_AI.m_Health <= 0) || (M2C_FIELD(arg1, s32 *, 0x48) & 2)) {
        var_v1 = 1;
    }
    return var_v1;
}
