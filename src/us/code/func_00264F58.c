#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"

extern f32 Random_Blend(f32 min, f32 max); /* random float in [min, max] */
extern f32 AI_DistSqToPointAtHeight(void *obj, CVector3 v);
extern f32 func_00298470(f32 x); /* sqrtf */
extern f32 func_002982F0(f32 x); /* sinf */
extern f32 func_002974C0(f32 x); /* cosf */

extern f32 gThrowableCollisionInfo_m_GravityAcceleration; /* gravity */

typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ CGameObjectInstance *owner;
} OwnedState;

/* Launch velocity: thrown away from the owner, aimed at its target. */
void Throwable_FlyingEnter(CGameObjectInstance *obj, OwnedState *state) {
    CGameObjectInstance *owner = state->owner;
    CGameObjectInstance *target;
    f32 speed;
    f32 lift;
    f32 r;
    f32 x;

    if (owner->m_AI.m_Mode != 0x15) {
        speed = 51.199997f;
        lift = 204.79999f;
    } else if ((target = owner->m_AI.m_pSightTarget) != NULL) {
        r = Random_Blend(1.5f, 2.5f);
        speed = r * func_00298470(AI_DistSqToPointAtHeight(obj, target->header.m_vPos));
        lift = -gThrowableCollisionInfo_m_GravityAcceleration / (r * 3.8f);
    } else {
        speed = 512.0f;
        lift = 358.4f;
    }
    lift += Random_Blend(0.0f, 2.56f);
    x = speed * func_002982F0(owner->m_RotY + 3.1415927f);
    obj->m_vVelocity.y = lift;
    obj->m_vVelocity.x = x;
    obj->m_vVelocity.z = speed * func_002974C0(owner->m_RotY + 3.1415927f);
}
