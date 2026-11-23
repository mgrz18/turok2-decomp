#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"
#include "globals/collision.h"
#include "protos/ai.h"

extern void Throwable_GetCarryPoint(void *obj, void *state, f32 *out);
extern sptr CGameObjectInstance__PickRandomAnim(void *actor, sptr action, sptr skip);
extern void CVector3__Lerp(f32 *out, f32 t, f32 *from, f32 *to);

void Throwable_PickedUpUpdate(CGameObjectInstance *obj, CAI *state) {
    f32 target[3];
    sptr anim;
    CGameObjectInstance *owner = state->m_Throwable.m_pOwner;

    gThrowableCollisionInfo_m_InstanceBehavior = 0;
    if (owner == NULL) {
        return;
    }
    anim = CGameObjectInstance__PickRandomAnim(owner, 10000, -1);
    if (owner->m_CurrentAnimation.m_nAnimIndex == anim
        && owner->m_CurrentAnimation.m_nFrames * 0.3f <= owner->m_CurrentAnimation.m_AnimationFrame) {
        Throwable_GetCarryPoint(obj, state, target);
        state->m_Throwable.m_TiltHeading = owner->m_RotY - obj->m_RotY;
        state->m_ConditionTimer += gFrameIncrement * 0.25f;
        if (state->m_ConditionTimer > 1.0f) {
            state->m_ConditionTimer = 1.0f;
        }
        CVector3__Lerp(&state->m_vDesiredPos.x, state->m_ConditionTimer, &obj->header.m_vPos.x, target);
        if (owner->m_CurrentAnimation.m_AnimationFrame < owner->m_CurrentAnimation.m_nFrames * 0.5f) {
            state->m_Throwable.m_TiltAngle += 0.087266475f;
        } else {
            state->m_Throwable.m_TiltAngle -= 0.087266475f;
        }
    }
    if (owner != NULL && owner->m_AI.m_Mode != 0x15) {
        CAI__SetMode(obj, state, 2);
    }
}
