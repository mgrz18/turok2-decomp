#include "common.h"
#include "globals/strings.h"
#include "structs/game_object.h"
#include "protos/iset.h"
#include "protos/cache.h"

/* The first section of an object model (CISet__GetBlock(*model, 0)): only its angle at 0x1E is read. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1E];
    /* 0x1E */ u16 angle;
} ModelSection;

f32 HalfToFloat(u16);                             /* extern */

f32 CGameObjectInstance__GetObjectScale(CGameObjectInstance *arg0) {
    f32 var_f0;
    f32 var_f20;
    sptr *temp_v0;

    var_f20 = 1.0f;
    if (arg0->m_dwFlags & 0x40000) {
        temp_v0 = CCache__Request(0, (sptr) arg0->m_rpObject, arg0->m_rpObjectHeader, 4, 0, 0, &sCGameObjectInstanceCacheDesc_AnimObjectInfo, 1);
        var_f0 = var_f20;
        if (temp_v0 != NULL) {
            var_f20 = HalfToFloat(((ModelSection *) CISet__GetBlock(*temp_v0, 0))->angle);
            CCache__Unlock(0, temp_v0);
            goto block_3;
        }
    } else {
block_3:
        var_f0 = var_f20;
    }
    return var_f0;
}
