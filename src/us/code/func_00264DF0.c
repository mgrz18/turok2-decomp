#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"
#include "globals/collision.h"
#include "protos/ai.h"

extern void Throwable_GetCarryPoint(void *obj, void *state, f32 *out);
extern sptr CGameObjectInstance__PickRandomAnim(void *actor, sptr action, sptr skip);
extern void CVector3__Lerp(f32 *out, f32 t, f32 *from, f32 *to);

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
    /* 0x000 */ u8 pad0[0x54];
    /* 0x054 */ f32 pos[3];
    /* 0x060 */ u8 pad60[4];
    /* 0x064 */ f32 blend;
    /* 0x068 */ u8 pad68[0xA4];
    /* 0x10C */ CGameObjectInstance *owner;
    /* 0x110 */ f32 tiltAngle;
    /* 0x114 */ f32 tiltHeading;
} OwnedState;
#endif

void Throwable_PickedUpUpdate(CGameObjectInstance *obj, OwnedState *state) {
    f32 target[3];
    sptr anim;
    CGameObjectInstance *owner = state->owner;

    gThrowableCollisionInfo_m_InstanceBehavior = 0;
    if (owner == NULL) {
        return;
    }
    anim = CGameObjectInstance__PickRandomAnim(owner, 10000, -1);
    if (owner->m_CurrentAnimation.m_nAnimIndex == anim
        && owner->m_CurrentAnimation.m_nFrames * 0.3f <= owner->m_CurrentAnimation.m_AnimationFrame) {
        Throwable_GetCarryPoint(obj, state, target);
        state->tiltHeading = owner->m_RotY - obj->m_RotY;
        state->blend += gFrameIncrement * 0.25f;
        if (state->blend > 1.0f) {
            state->blend = 1.0f;
        }
        CVector3__Lerp(state->pos, state->blend, &obj->header.m_vPos.x, target);
        if (owner->m_CurrentAnimation.m_AnimationFrame < owner->m_CurrentAnimation.m_nFrames * 0.5f) {
            state->tiltAngle += 0.087266475f;
        } else {
            state->tiltAngle -= 0.087266475f;
        }
    }
    if (owner != NULL && owner->m_AI.m_Mode != 0x15) {
        CAI__SetMode(obj, state, 2);
    }
}
