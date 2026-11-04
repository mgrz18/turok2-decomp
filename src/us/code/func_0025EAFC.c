#include "common.h"
#include "structs/game_object.h"

extern f32 D_800A77EC;
extern f32 D_800A77F0;

f32 func_0025EAFC(CGameObjectInstance *obj) {
    f32 var_f0;

    var_f0 = D_800A77EC;
    if ((u16) obj->m_nTypeFlag != 0x259) {
        var_f0 = D_800A77F0;
    }
    return var_f0;
}
