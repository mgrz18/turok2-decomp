#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"

extern s32 D_800F1B40;   /* collision info */
extern f32 func_002671B4(CGameObjectInstance *obj);   /* the object's height */
extern s32 func_00239F00(CGameObjectInstance *obj, CVector3 to, void *info);

/* Probes a move of obj, raised by half its height plus its step height, to
 * *to; writes where it ends up back to *to and returns 1 if that differs
 * from the target. The object itself is left as it was. */
s32 func_00256B84(CGameObjectInstance *obj, CVector3 *to) {
    CObjectHead saved;
    s32 blocked;

    saved = *(CObjectHead *)obj;
    obj->header.m_vPos.y += func_002671B4(obj) * 0.5f;
    obj->header.m_vPos.y += obj->m_CollisionYOffset;
    func_00239F00(obj, *to, &D_800F1B40);
    blocked = 0;
    if (to->x != obj->header.m_vPos.x || to->y != obj->header.m_vPos.y || to->z != obj->header.m_vPos.z) {
        blocked = 1;
    }
    *to = obj->header.m_vPos;
    *(CObjectHead *)obj = saved;
    return blocked;
}
