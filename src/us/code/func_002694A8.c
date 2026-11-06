#include "common.h"
#include "structs/game_object.h"


typedef struct CollisionInfo {
    /* 0x00 */ s32 flags;
} CollisionInfo;

extern CollisionInfo D_800F1BE0;

s32 GameObject_CanEnterWater(CGameObjectInstance *actor);
s32 func_00267F10(CGameObjectInstance *actor);
f32 CGameRegion__GetHeight(void *region, f32 x, f32 z);
void Collision_MoveInstance(CGameObjectInstance *actor, CVector3 pos, CollisionInfo *info);

/* Slides an actor to (x, z) with collision, keeping its height above the floor. */
void GameObject_SlideTo(CGameObjectInstance *actor, f32 x, f32 z) {
    CVector3 savedVelocity;
    CVector3 to;
    s32 noA;
    s32 noB;
    s32 oldFlags;
    f32 height;

    noA = GameObject_CanEnterWater(actor) == 0;
    noB = func_00267F10(actor) == 0;
    height = actor->header.m_vPos.y - CGameRegion__GetHeight(actor->header.m_pCurrentRegion, actor->header.m_vPos.x, actor->header.m_vPos.z);
    savedVelocity = actor->m_vVelocity;
    actor->m_vVelocity.x = actor->m_vVelocity.y = actor->m_vVelocity.z = 0.0f;
    to.x = x;
    to.y = actor->header.m_vPos.y;
    to.z = z;
    oldFlags = D_800F1BE0.flags;
    if (noA) {
        D_800F1BE0.flags = oldFlags & ~0x40000;
    } else {
        D_800F1BE0.flags = oldFlags | 0x40000;
    }
    if (noB) {
        D_800F1BE0.flags &= ~0x30000;
    } else {
        D_800F1BE0.flags |= 0x30000;
    }
    Collision_MoveInstance(actor, to, &D_800F1BE0);
    D_800F1BE0.flags = oldFlags;
    actor->m_vVelocity = savedVelocity;
    actor->header.m_vPos.y = CGameRegion__GetHeight(actor->header.m_pCurrentRegion, actor->header.m_vPos.x, actor->header.m_vPos.z) + height;
}
