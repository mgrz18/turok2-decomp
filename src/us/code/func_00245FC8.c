#include "common.h"
#include "structs/game_object.h"

/* The leading 0x34 bytes of a game object: what the collision test reads. */
typedef struct {
    /* 0x00 */ CInstanceHeader header;
    /* 0x18 */ CVector3 m_vVelocity;
    /* 0x24 */ CVector3 m_vWaterDisplacement;
    /* 0x30 */ struct CGameObjectInstance *m_pInstanceBelow;
} ObjectHead; /* size 0x34 */

struct CollisionInfo;
extern struct CollisionInfo D_800F1B40;
extern CGameObjectInstance *D_800F1CE0; /* the instance the last test hit */
f32 func_002671B4(CGameObjectInstance *obj); /* the object's height */
s32 func_00239F00(void *obj, CVector3 to, struct CollisionInfo *info);

/* Whether the line from the top of `from` to the top of `target` is blocked,
 * by something other than `target` itself. */
s32 func_00245FC8(CGameObjectInstance *target, s32 unused, CGameObjectInstance *from) {
    ObjectHead saved;
    CVector3 to;
    s32 hit;

    saved = *(ObjectHead *) from;
    from->header.m_vPos.y += func_002671B4(from);
    from->header.m_vPos.y += from->m_CollisionYOffset;
    to = target->header.m_vPos;
    to.y += func_002671B4(target);
    to.y += target->m_CollisionYOffset;
    hit = func_00239F00(from, to, &D_800F1B40);
    if (D_800F1CE0 == target) {
        hit = 0;
    }
    *(ObjectHead *) from = saved;
    return hit;
}
