#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"

/* How far the highest corner of the current region is above the object. */
f32 GameObject_GetRegionTopAbove(CGameObjectInstance *arg0) {
    f32 temp_f1;
    f32 var_f0;
    f32 var_f1;
    CGameRegion *temp_a1;

    temp_a1 = arg0->header.m_pCurrentRegion;
    var_f0 = temp_a1->m_pVertices[1]->y;
    temp_f1 = temp_a1->m_pVertices[0]->y;
    if (!(temp_f1 <= var_f0)) {
        var_f0 = temp_f1;
    }
    var_f1 = temp_a1->m_pVertices[2]->y;
    if (!(var_f0 <= var_f1)) {
        var_f1 = var_f0;
    }
    return var_f1 - arg0->header.m_vPos.y;
}
