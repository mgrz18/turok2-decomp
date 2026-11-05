#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

typedef struct CollisionInfo {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 pad6[0x1C - 0x6];
} CollisionInfo; /* size 0x1C */

extern CCollisionResults D_800F1CE0;

s32 func_00239F00(CObjectHead *obj, CVector3 to, CollisionInfo *info);

/* Whether obj can move to `to` without hitting anything; obj is left where it was. */
s32 func_002698AC(CObjectHead *obj, CVector3 to, CollisionInfo *info) {
    CObjectHead saved;
    CollisionInfo probe;
    s32 hit;

    saved = *obj;
    probe = *info;
    probe.unk5 = 1;
    probe.unk4 = 0;
    func_00239F00(obj, to, &probe);
    *obj = saved;
    hit = 0;
    if (D_800F1CE0.m_pInstance != NULL || D_800F1CE0.m_pWallRegion != NULL) {
        hit = 1;
    }
    return hit ^ 1;
}
