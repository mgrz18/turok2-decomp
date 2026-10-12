#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"

s32 func_002565DC(CPlayer *arg0) {
    s32 var_v0;

    var_v0 = 1;
    if (!(arg0->m_DesiredJumpAngle > 5.12f)) {
        var_v0 = 0;
    }
    return var_v0;
}
