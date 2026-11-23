#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "globals/collision.h"
#include "structs/collision.h"



s32 GameObject_CanClimbUp(CGameObjectInstance *actor);
s32 GameObject_CanEnterWater(CGameObjectInstance *actor);
s32 GameObject_CanLeaveWater(CGameObjectInstance *actor);
s32 Collision_MoveInstance(CObjectHead *obj, CVector3 to, CCollisionInfo *info);

/* Tests moving an actor to pos with the collision settings that suit it, leaving it where it was; returns the collision result. */
s32 GameObject_TestMove(CGameObjectInstance *actor, CVector3 pos, CCollisionInfo **outInfo) {
    CObjectHead saved;
    CCollisionInfo *info;
    s32 alt;
    s32 hit;

    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        info = &gCollisionInfoTestMoveLand;
    } else {
        alt = GameObject_CanClimbUp(actor);
        if (!GameObject_CanEnterWater(actor)) {
            info = alt ? &gCollisionInfoTestMoveClimb : &gCollisionInfoTestMoveLand;
        } else {
            info = !GameObject_CanLeaveWater(actor) ? &gCollisionInfoTestMoveWater : &gCollisionInfoTestMove;
        }
    }
    if (outInfo != NULL) {
        *outInfo = info;
    }
    saved = *(CObjectHead *) actor;
    hit = Collision_MoveInstance((CObjectHead *) actor, pos, info);
    *(CObjectHead *) actor = saved;
    return hit;
}
