#include "common.h"
#include "structs/game_object.h"

/* The leading 0x34 bytes of a game object: what the collision test reads. */
typedef struct {
    /* 0x00 */ CInstanceHeader header;
    /* 0x18 */ CVector3 m_vVelocity;
    /* 0x24 */ CVector3 m_vWaterDisplacement;
    /* 0x30 */ struct CGameObjectInstance *m_pInstanceBelow;
} ObjectHead; /* size 0x34 */

/* What the collision test hit: its first instance, flags at 0x2. */
typedef struct {
    u8 pad0[2];
    u16 flags; /* 0x2 */
} HitInstance;

struct CollisionInfo;
extern struct CollisionInfo D_800F1AE0;
extern HitInstance *D_800F1D7C;
extern HitInstance *D_800F1D94;
f32 func_002671B4(CGameObjectInstance *obj); /* the object's height */
s32 func_00239F00(void *obj, CVector3 to, struct CollisionInfo *info);

/* Whether the line between the two objects' eyes (0.8 of their height up)
 * hits something flagged 2. */
s32 func_00245E48(CGameObjectInstance *from, s32 unused, CGameObjectInstance *to) {
    ObjectHead saved;
    CVector3 target;

    saved = *(ObjectHead *) from;
    from->header.m_vPos.y += func_002671B4(from) * 0.8f;
    from->header.m_vPos.y += from->m_CollisionYOffset;
    target = to->header.m_vPos;
    target.y += func_002671B4(to) * 0.8f;
    target.y += to->m_CollisionYOffset;
    func_00239F00(from, target, &D_800F1AE0);
    do {
        *(ObjectHead *) from = saved;
    } while (0);
    if (D_800F1D7C != NULL && (D_800F1D7C->flags & 2)) {
        return 1;
    }
    if (D_800F1D94 != NULL && (D_800F1D94->flags & 2)) {
        return 1;
    }
    return 0;
}
