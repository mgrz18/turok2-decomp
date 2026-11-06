#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

extern CCollisionResults *gCollisionResultsP;     /* current hit buffer */
typedef struct CollisionInfo {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s8 flagB;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s8 flagA;
} CollisionInfo;

extern CollisionInfo D_800F1CC0;
extern u8 gScene_m_ActorPool[];

void Collision_MoveInstance(CInstanceHeader *actor, CVector3 pos, void *info);
CVector3 CVector3__RandomPerturb(CVector3 *in, f32 amount);
void CVector3__Normalize(CVector3 *v);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
u8 *CLevelActorPool__Spawn(void *table, void *slot, s32 kind, CVector3 velocity, CVector3 pos, void *region, f32 arg);

/* GameObject_TraceMove's logic, inlined with the collision flags addressed directly. */
static __inline__ void tracePos(CInstanceHeader *actor, CVector3 pos, CVector3 *outPos, void **outRegion, s32 flagA, s32 flagB) {
    CInstanceHeader saved;
    CVector3 end;
    CCollisionResults hits;
    CCollisionResults *oldHits;
    s32 oldA;
    s32 oldB;
    void *region;

    saved = *actor;
    oldA = D_800F1CC0.flagA;
    oldB = D_800F1CC0.flagB;
    oldHits = gCollisionResultsP;
    D_800F1CC0.flagA = flagA;
    D_800F1CC0.flagB = flagB;
    gCollisionResultsP = &hits;
    Collision_MoveInstance(actor, pos, &D_800F1CC0);
    actor->m_vPos.y = pos.y;
    end.x = actor->m_vPos.x;
    end.y = pos.y;
    end.z = actor->m_vPos.z;
    region = actor->m_pCurrentRegion;
    gCollisionResultsP = oldHits;
    *actor = saved;
    *outPos = end;
    *outRegion = region;
    D_800F1CC0.flagA = oldA;
    D_800F1CC0.flagB = oldB;
}

/* Spawns a particle of the given kind where an actor would end up at pos. */
void GameObject_DropPickupAt(CInstanceHeader *actor, CVector3 pos, s32 kind, f32 arg) {
    CVector3 up;
    CVector3 dir;
    CVector3 velocity;
    CVector3 at;
    void *region;

    tracePos(actor, pos, &at, &region, 0, 0);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    dir = CVector3__RandomPerturb(&up, 0.25f);
    CVector3__Normalize(&dir);
    CVector3__Scale(&velocity, &dir, 409.59998f);
    CLevelActorPool__Spawn(gScene_m_ActorPool, NULL, kind, velocity, at, region, arg);
}
