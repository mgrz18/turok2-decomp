#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ f32 y;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ CGameRegion *tri;
} Unk00268B98;

/* Height of arg0 above the lowest vertex of its triangle. */
f32 GameObject_GetHeightAboveRegionBottom(Unk00268B98 *arg0) {
    f32 temp_f1;
    f32 var_f0;
    f32 var_f1;
    CGameRegion *temp_a1;

    temp_a1 = arg0->tri;
    var_f0 = temp_a1->m_pVertices[1]->y;
    temp_f1 = temp_a1->m_pVertices[0]->y;
    if (!(var_f0 <= temp_f1)) {
        var_f0 = temp_f1;
    }
    var_f1 = temp_a1->m_pVertices[2]->y;
    if (!(var_f1 <= var_f0)) {
        var_f1 = var_f0;
    }
    return arg0->y - var_f1;
}
