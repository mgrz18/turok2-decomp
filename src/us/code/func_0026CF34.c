#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/rect.h"

/* The XZ bounding rectangle of the triangle. */
void func_0026CF34(CGameRegion *arg0, CRect *arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 var_f0;
    f32 var_f0_2;
    f32 var_f0_3;
    f32 var_f0_4;
    f32 var_f1;
    f32 var_f1_2;
    f32 var_f1_3;
    f32 var_f1_4;

    var_f1 = arg0->m_pVertices[1]->x;
    temp_f0 = arg0->m_pVertices[0]->x;
    if (!(var_f1 <= temp_f0)) {
        var_f1 = temp_f0;
    }
    var_f0 = arg0->m_pVertices[2]->x;
    if (!(var_f0 <= var_f1)) {
        var_f0 = var_f1;
    }
    arg1->minX = var_f0;
    var_f1_2 = arg0->m_pVertices[1]->x;
    temp_f0_2 = arg0->m_pVertices[0]->x;
    if (!(temp_f0_2 <= var_f1_2)) {
        var_f1_2 = temp_f0_2;
    }
    var_f0_2 = arg0->m_pVertices[2]->x;
    if (!(var_f1_2 <= var_f0_2)) {
        var_f0_2 = var_f1_2;
    }
    arg1->maxX = var_f0_2;
    var_f1_3 = arg0->m_pVertices[1]->z;
    temp_f0_3 = arg0->m_pVertices[0]->z;
    if (!(var_f1_3 <= temp_f0_3)) {
        var_f1_3 = temp_f0_3;
    }
    var_f0_3 = arg0->m_pVertices[2]->z;
    if (!(var_f0_3 <= var_f1_3)) {
        var_f0_3 = var_f1_3;
    }
    arg1->minZ = var_f0_3;
    var_f1_4 = arg0->m_pVertices[1]->z;
    temp_f0_4 = arg0->m_pVertices[0]->z;
    if (!(temp_f0_4 <= var_f1_4)) {
        var_f1_4 = temp_f0_4;
    }
    var_f0_4 = arg0->m_pVertices[2]->z;
    if (!(var_f1_4 <= var_f0_4)) {
        var_f0_4 = var_f1_4;
    }
    arg1->maxZ = var_f0_4;
}
