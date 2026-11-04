#include "common.h"
#include "structs/game_object.h"

s32 func_0026803C(CGameObjectInstance *obj) {
    s32 var_v1;
    CIntelligence *temp_a1;

    temp_a1 = obj->header.m_pIntelligence;
    if (temp_a1->m_Type == 1) {
        var_v1 = 0;
        if ((obj->m_AI.m_Health <= 0) || (temp_a1->m_Enemy.m_dwEnemyFlags & 0x10)) {
            var_v1 = 1;
        }
        return var_v1;
    }
    return 0;
}
