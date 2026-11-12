#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "protos/ai.h"

extern f32 AI_FlatDistSqToPoint(void *actor, CVector3 point); /* squared XZ distance */
extern f32 GameObject_GetCollisionRadius(void *actor);              /* collision radius */
extern void EnemyAI_SetGrabbed(void *actor, void *ai, sptr arg2);
extern f32 func_00298470(f32 x); /* sqrtf */

extern s8 gThrowableCollisionInfo_m_InstanceBehavior[]; /* local type: globals/collision.h declares it; this view (its type here) is what keeps the ROM's code */

#ifdef TARGET_PORT
/* TARGET_PORT: the throwable's vars in CAI::m_VarBuffer. On the N64: the owner at 0x10C
 * ([0]), the tilt angle at 0x110 ([1]), its heading at 0x114 ([2]). The host owner is
 * pointer-wide (m_VarPtr[0], [0..1]), so the tilt moves to m_VarF32[2]/[3] (STABILITY.md S14). */
typedef union {
    struct { u8 _h_pos[__builtin_offsetof(CAI, m_vDesiredPos)]; f32 pos[3]; };
    struct { u8 _h_blend[__builtin_offsetof(CAI, m_ConditionTimer)]; f32 blend; };
    struct { u8 _h_owner[__builtin_offsetof(CAI, m_VarPtr[0])]; CGameObjectInstance *owner; };
    struct { u8 _h_tilt[__builtin_offsetof(CAI, m_VarF32[2])]; f32 tiltAngle; };
    struct { u8 _h_head[__builtin_offsetof(CAI, m_VarF32[3])]; f32 tiltHeading; };
} OwnedState;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ CGameObjectInstance *owner;
    /* 0x110 */ f32 tiltAngle;
} OwnedState;
#endif

void Throwable_FlyingUpdate(CGameObjectInstance *obj, OwnedState *state) {
    CGameObjectInstance *owner = state->owner;
    f32 distSq;

    if (owner != NULL) {
        gThrowableCollisionInfo_m_InstanceBehavior[0] = 0;
        distSq = AI_FlatDistSqToPoint(obj, owner->header.m_vPos);
        if ((GameObject_GetCollisionRadius(obj) + GameObject_GetCollisionRadius(owner)) * (GameObject_GetCollisionRadius(obj) + GameObject_GetCollisionRadius(owner)) < distSq) {
            state->owner = NULL;
#ifdef TARGET_PORT
            EnemyAI_SetGrabbed(owner, &owner->m_AI, 0); /* the CAI at its host offset */
#else
            EnemyAI_SetGrabbed(owner, (u8 *) owner + 0x140, 0);
#endif
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
