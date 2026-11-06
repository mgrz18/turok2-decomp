#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"

typedef struct CollisionInfo {
    /* 0x00 */ s32 flags;
} CollisionInfo;

extern CollisionInfo gCollisionInfoTestMove;
extern CollisionInfo D_800F1B60;
extern CollisionInfo gCollisionInfoTestMoveWater;
extern CollisionInfo D_800F1BE0;

s32 func_00267EE8(CGameObjectInstance *actor);
s32 GameObject_CanEnterWater(CGameObjectInstance *actor);
s32 GameObject_CanLeaveWater(CGameObjectInstance *actor);
s32 Collision_MoveInstance(CGameObjectInstance *actor, CVector3 pos, CollisionInfo *info);

/* Tests moving an actor to pos with the collision settings that suit it, leaving it where it was; returns the collision result. */
s32 GameObject_TestMove(CGameObjectInstance *actor, CVector3 pos, CollisionInfo **outInfo) {
    CObjectHead saved;
    CollisionInfo *info;
    s32 alt;
    s32 hit;

    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        info = &D_800F1BE0;
    } else {
        alt = func_00267EE8(actor);
        if (!GameObject_CanEnterWater(actor)) {
            info = alt ? &D_800F1B60 : &D_800F1BE0;
        } else {
            info = !GameObject_CanLeaveWater(actor) ? &gCollisionInfoTestMoveWater : &gCollisionInfoTestMove;
        }
    }
    if (outInfo != NULL) {
        *outInfo = info;
    }
    saved = *(CObjectHead *) actor;
    hit = Collision_MoveInstance(actor, pos, info);
    *(CObjectHead *) actor = saved;
    return hit;
}
