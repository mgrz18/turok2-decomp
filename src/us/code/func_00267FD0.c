#include "common.h"
#include "structs/game_object.h"

/* For type-1 objects: whether dead (or flagged 0x800). */
s32 func_00267FD0(CGameObjectInstance *arg0) {
    s32 var_v1;
    struct CIntelligence *temp_a1;

    temp_a1 = arg0->header.m_pIntelligence;
    if (temp_a1->m_Type == 1) {
        var_v1 = 0;
        if ((arg0->m_AI.m_Health <= 0) || (temp_a1->m_Enemy.m_dwEnemyFlags & 0x800)) {
            var_v1 = 1;
        }
        return var_v1;
    }
    return 0;
}
