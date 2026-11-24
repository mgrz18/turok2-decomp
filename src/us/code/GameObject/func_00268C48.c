#include "common.h"
#include "protos/game_object.h"
#include "structs/vector.h"
#include "structs/collision.h"
#include "globals/scene.h"
#include "globals/collision.h"



u8 *CLevelActorPool__Spawn(CLevelActorPool *table, void *slot, s32 kind, CVector3 velocity, CVector3 pos, CGameRegion *region, f32 arg);

/* GameObject_TraceMove's logic, inlined with the collision flags addressed directly. */
static __inline__ void tracePos(CInstanceHeader *actor, CVector3 pos, CVector3 *outPos, CGameRegion **outRegion, s32 flagA, s32 flagB) {
    CInstanceHeader saved;
    CVector3 end;
    CCollisionResults hits;
    CCollisionResults *oldHits;
    s32 oldA;
    s32 oldB;
    CGameRegion *region;

    saved = *actor;
    oldA = gCollisionInfoTrace.m_GroundBehavior;
    oldB = gCollisionInfoTrace.m_InstanceBehavior;
    oldHits = gCollisionResultsP;
    gCollisionInfoTrace.m_GroundBehavior = flagA;
    gCollisionInfoTrace.m_InstanceBehavior = flagB;
    gCollisionResultsP = &hits;
    Collision_MoveInstance((CGameObjectInstance *) actor, pos, &gCollisionInfoTrace);
    actor->m_vPos.y = pos.y;
    end.x = actor->m_vPos.x;
    end.y = pos.y;
    end.z = actor->m_vPos.z;
    region = actor->m_pCurrentRegion;
    gCollisionResultsP = oldHits;
    *actor = saved;
    *outPos = end;
    *outRegion = region;
    gCollisionInfoTrace.m_GroundBehavior = oldA;
    gCollisionInfoTrace.m_InstanceBehavior = oldB;
}

/* Spawns a particle of the given kind where an actor would end up at pos. */
void GameObject_DropPickupAt(CInstanceHeader *actor, CVector3 pos, s32 kind, f32 arg) {
    CVector3 up;
    CVector3 dir;
    CVector3 velocity;
    CVector3 at;
    CGameRegion *region;

    tracePos(actor, pos, &at, &region, 0, 0);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    dir = CVector3__RandomPerturb(&up, 0.25f);
    CVector3__Normalize(&dir);
    CVector3__Scale(&velocity, &dir, 409.59998f);
    CLevelActorPool__Spawn(&gScene.m_ActorPool, NULL, kind, velocity, at, region, arg);
}
