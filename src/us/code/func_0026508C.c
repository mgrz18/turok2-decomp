#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"

extern f32 AI_FlatDistSqToPoint(void *actor, CVector3 point); /* squared XZ distance */
extern f32 GameObject_GetCollisionRadius(void *actor);              /* collision radius */
extern void EnemyAI_SetGrabbed(void *actor, void *ai, s32 arg2);
extern void CAI__SetMode(void *obj, void *state, s32 mode);
extern f32 func_00298470(f32 x); /* sqrtf */

extern s8 gThrowableCollisionInfo_m_InstanceBehavior[]; /* local type: globals/collision.h declares it; this view (its type here) is what keeps the ROM's code */

typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ CGameObjectInstance *owner;
    /* 0x110 */ f32 tiltAngle;
} OwnedState;

void Throwable_FlyingUpdate(CGameObjectInstance *obj, OwnedState *state) {
    CGameObjectInstance *owner = state->owner;
    f32 distSq;

    if (owner != NULL) {
        gThrowableCollisionInfo_m_InstanceBehavior[0] = 0;
        distSq = AI_FlatDistSqToPoint(obj, owner->header.m_vPos);
        if ((GameObject_GetCollisionRadius(obj) + GameObject_GetCollisionRadius(owner)) * (GameObject_GetCollisionRadius(obj) + GameObject_GetCollisionRadius(owner)) < distSq) {
            state->owner = NULL;
            EnemyAI_SetGrabbed(owner, (u8 *) owner + 0x140, 0);
        }
    } else {
        gThrowableCollisionInfo_m_InstanceBehavior[0] = 2;
    }
    if (obj->m_vVelocity.x == 0.0f && obj->m_vVelocity.y == 0.0f && obj->m_vVelocity.z == 0.0f) {
        CAI__SetMode(obj, state, 0);
    }
    state->tiltAngle += func_00298470(obj->m_vVelocity.x * obj->m_vVelocity.x + obj->m_vVelocity.y * obj->m_vVelocity.y + obj->m_vVelocity.z * obj->m_vVelocity.z)
                        * 0.00021305290f;
}
