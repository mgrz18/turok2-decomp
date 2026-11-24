#include "common.h"
#include "protos/game_object.h"
#include "structs/vector.h"
#include "structs/collision.h"
#include "globals/collision.h"



/* Moves an actor to pos with collision, without moving it: returns where it would end up and in which region. */
void GameObject_TraceMove(CInstanceHeader *actor, CVector3 pos, CVector3 *outPos, CGameRegion **outRegion, s32 flagA, s32 flagB) {
    CInstanceHeader saved;
    CVector3 end;
    CCollisionResults hits;
    CCollisionResults *oldHits;
    s32 oldA;
    s32 oldB;
    CGameRegion *region;
    s8 *flags;

    saved = *actor;
    oldHits = gCollisionResultsP;
    gCollisionResultsP = &hits;
    flags = &gCollisionInfoTrace.m_GroundBehavior;
    oldA = flags[0];
    oldB = flags[-2];
    flags[0] = flagA;
    flags[-2] = flagB;
    Collision_MoveInstance((CGameObjectInstance *) actor, pos, (CCollisionInfo *) (flags - 6));
    gCollisionResultsP = oldHits;
    actor->m_vPos.y = pos.y;
    end.x = actor->m_vPos.x;
    end.y = pos.y;
    end.z = actor->m_vPos.z;
    region = actor->m_pCurrentRegion;
    *actor = saved;
    *outPos = end;
    *outRegion = region;
    flags[0] = oldA;
    flags[-2] = oldB;
}
