#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"

extern void Throwable_GetCarryPoint(void *obj, void *state, f32 *out);
extern s32 CGameObjectInstance__PickRandomAnim(void *actor, s32 action, s32 skip);
extern void CVector3__Lerp(f32 *out, f32 t, f32 *from, f32 *to);
extern void CAI__SetMode(void *obj, void *state, s32 mode);

extern s8 gThrowableCollisionInfo_m_InstanceBehavior;

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

void Throwable_PickedUpUpdate(CGameObjectInstance *obj, OwnedState *state) {
    f32 target[3];
    s32 anim;
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
