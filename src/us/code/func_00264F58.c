#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "globals/collision.h"

extern f32 Random_Blend(f32 min, f32 max); /* random float in [min, max] */
extern f32 AI_DistSqToPointAtHeight(void *obj, CVector3 v);
extern f32 func_00298470(f32 x); /* sqrtf */
extern f32 func_002982F0(f32 x); /* sinf */
extern f32 func_002974C0(f32 x); /* cosf */

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
} OwnedState;
#endif

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
