#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

f32 func_00232ECC(CGameObjectInstance *arg0) {
    f32 var_f0;

    var_f0 = 0.0f;
    if (M2C_FIELD(M2C_FIELD(arg0, void **, 0xF8), s32 *, 0x14) == 0) {
        var_f0 = arg0->m_AI.m_vFlinchAxis.z;
    }
    return var_f0;
}
