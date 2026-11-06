#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"

s32 CPlayer__IsCrouched(CPlayer *arg0) {
    s32 var_v0;

    var_v0 = 1;
    if (!(arg0->m_DuckOffset > 5.12f)) {
        var_v0 = 0;
    }
    return var_v0;
}
