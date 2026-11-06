#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "globals/collision.h"

typedef struct CollisionInfo {
    /* 0x00 */ s32 flags;
} CollisionInfo;


f32 GameObject_GetCollisionHeight(CGameObjectInstance *actor);    /* height */
s32 Collision_MoveInstance(CGameObjectInstance *actor, CVector3 pos, CollisionInfo *info);

/* Whether actor can see target: a clear line from 80% of one's height to 80% of the other's. */
s32 GameObject_CanSee(CGameObjectInstance *actor, CGameObjectInstance *target) {
    CObjectHead saved;
    CVector3 to;
    s32 hit;

    saved = *(CObjectHead *) actor;
    actor->header.m_vPos.y += GameObject_GetCollisionHeight(actor) * 0.8f;
    to = target->header.m_vPos;
    to.y += GameObject_GetCollisionHeight(target) * 0.8f;
    hit = Collision_MoveInstance(actor, to, &gCollisionInfoSight);
    *(CObjectHead *) actor = saved;
    return hit == 0;
}
