#include "common.h"
#include "structs/game_object.h"


f32 EnemyAI_GetTypeTurnRate(CGameObjectInstance *obj) {
    f32 var_f0;

    var_f0 = 1.5707965f;
    if ((u16) obj->m_nTypeFlag != 0x259) {
        var_f0 = 0.26179942f;
    }
    return var_f0;
}
