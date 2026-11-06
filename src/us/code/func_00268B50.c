#include "common.h"
#include "structs/game_object.h"

/* The region (floor triangle) an object stands in: its three corners at 4, 8, 0xC. */
struct CGameRegion {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ CVector3 *m_pCorners[3];
    /* 0x10 */ u8 pad10[0x1C - 0x10];
    /* 0x1C */ u32 unk1C; /* visibility bits, see CCamera__UpdateVisBits */
};

/* How far the highest corner of the current region is above the object. */
f32 GameObject_GetRegionTopAbove(CGameObjectInstance *arg0) {
    f32 temp_f1;
    f32 var_f0;
    f32 var_f1;
    struct CGameRegion *temp_a1;

    temp_a1 = arg0->header.m_pCurrentRegion;
    var_f0 = temp_a1->m_pCorners[1]->y;
    temp_f1 = temp_a1->m_pCorners[0]->y;
    if (!(temp_f1 <= var_f0)) {
        var_f0 = temp_f1;
    }
    var_f1 = temp_a1->m_pCorners[2]->y;
    if (!(var_f0 <= var_f1)) {
        var_f1 = var_f0;
    }
    return var_f1 - arg0->header.m_vPos.y;
}
