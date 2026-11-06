#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"
#include "globals/collision.h"

typedef struct CollisionInfo {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 pad6[0x1C - 0x6];
} CollisionInfo; /* size 0x1C */


s32 Collision_MoveInstance(CObjectHead *obj, CVector3 to, CollisionInfo *info);

/* Whether obj can move to `to` without hitting anything; obj is left where it was. */
s32 GameObject_CanMoveTo(CObjectHead *obj, CVector3 to, CollisionInfo *info) {
    CObjectHead saved;
    CollisionInfo probe;
    s32 hit;

    saved = *obj;
    probe = *info;
    probe.unk5 = 1;
    probe.unk4 = 0;
    Collision_MoveInstance(obj, to, &probe);
    *obj = saved;
    hit = 0;
    if (gCollisionResults.m_pInstance != NULL || gCollisionResults.m_pWallRegion != NULL) {
        hit = 1;
    }
    return hit ^ 1;
}
