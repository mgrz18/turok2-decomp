#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"

/* The highest y of the triangle's three vertices. */
f32 func_0026D028(CGameRegion *arg0) {
    f32 temp_f0;
    f32 var_f0;
    f32 var_f1;

    var_f1 = arg0->m_pVertices[1]->y;
    temp_f0 = arg0->m_pVertices[0]->y;
    if (!(temp_f0 <= var_f1)) {
        var_f1 = temp_f0;
    }
    var_f0 = arg0->m_pVertices[2]->y;
    if (!(var_f1 <= var_f0)) {
        var_f0 = var_f1;
    }
    return var_f0;
}
