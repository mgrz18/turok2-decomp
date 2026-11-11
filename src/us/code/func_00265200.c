/* Boundary fixed in versions/cuts.us.txt: the function starts at 0x00265200 (its pointer in the data is
 * 0x00265200). The word splat puts at 0x002651FC, `jal CParticleSystem__Free`, is a
 * stray word ahead of the prologue, never executed. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0020486C.c, func_0020486C, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"

typedef struct {
    s32 a;
    s32 b;
} EventArg;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 timer;
    /* 0x44 */ u8 pad44[0x20];
    /* 0x64 */ f32 duration;
    /* 0x68 */ u8 pad68[0x5E];
    /* 0xC6 */ u8 anim;
    /* 0xC7 */ s8 active;
} ActionState;

extern void ImpactEvent_Call(CGameObjectInstance *a, CGameObjectInstance *b, s32 kind, CVector3 pos, EventArg extra);

static __inline__ s32 animReached(CGameObjectInstance *actor, ActionState *state, f32 scale) {
    if (actor->m_nAnims == 1) return 1;
    if (actor->m_CurrentAnimation.m_nAnimIndex != (s8) state->anim || actor->m_CurrentAnimation.m_nRequestedAnimIndex != (s8) state->anim) return 0;
    if (state->active != 0 && !(actor->m_dwFlags & 0x400)) return 1;
    if (actor->m_CurrentAnimation.m_nFrames < 3 && (actor->m_dwFlags & 0x400)) {
        return (f32) actor->m_BlendLength * scale <= actor->m_BlendPercent;
    } else {
        return (f32) actor->m_CurrentAnimation.m_nFrames * scale <= actor->m_CurrentAnimation.m_AnimationFrame;
    }
}

#ifdef TARGET_PORT
/* Host body (C23): the N64 code reads the door's CAI through ActionState at N64 offsets (the
 * timer m_ModeTime 0x40, the duration m_ConditionTimer 0x64, m_AnimIndex 0xC6,
 * m_AnimRepeatedCount 0xC7). On the host CAI those offsets hold other fields: the "duration ran
 * out" test was true on the first update, so every opened door went straight back to mode 1
 * (closed) and no door ever opened. */
static __inline__ s32 animReachedHost(CGameObjectInstance *actor, CAI *ai, f32 scale) {
    if (actor->m_nAnims == 1) return 1;
    if (actor->m_CurrentAnimation.m_nAnimIndex != (s8) ai->m_AnimIndex || actor->m_CurrentAnimation.m_nRequestedAnimIndex != (s8) ai->m_AnimIndex) return 0;
    if (ai->m_AnimRepeatedCount != 0 && !(actor->m_dwFlags & 0x400)) return 1;
    if (actor->m_CurrentAnimation.m_nFrames < 3 && (actor->m_dwFlags & 0x400)) {
        return (f32) actor->m_BlendLength * scale <= actor->m_BlendPercent;
    } else {
        return (f32) actor->m_CurrentAnimation.m_nFrames * scale <= actor->m_CurrentAnimation.m_AnimationFrame;
    }
}

void Door_OpenUpdate(CGameObjectInstance *actor, CAI *ai) {
    CDoorIntelligence *info = &actor->header.m_pIntelligence->m_Door;
    EventArg extra;
    f32 t;

    if (info->m_OpenDoorEvent != -1) {
        if (animReachedHost(actor, ai, (f32) info->m_OpenDoorEvent * 0.007874016f)) {
            extra.a = 0;
            ImpactEvent_Call(actor, actor, 6, actor->header.m_vPos, extra);
            CScene__SetInstanceSavedBit(&gScene, actor, 1);
        }
    }
    if ((info->m_dwFlags & 1) && ai->m_AnimRepeatedCount != 0) {
        t = info->m_OpenDuration;
        if (t == 0.0f || ai->m_ConditionTimer < t) {
            if (ai->m_ConditionTimer == 0.0f) {
                ai->m_ModeTime = 0.0f;
            }
            ai->m_ConditionTimer = t;
        }
    }
    if (ai->m_ConditionTimer != 0.0f && ai->m_ConditionTimer <= ai->m_ModeTime) {
        CAI__SetMode(actor, ai, 1);
    }
}
#else
void Door_OpenUpdate(CGameObjectInstance *actor, ActionState *state) {
    CDoorIntelligence *info = &actor->header.m_pIntelligence->m_Door;
    EventArg extra;
    f32 t;

    if (info->m_OpenDoorEvent != -1) {
        if (animReached(actor, state, (f32) info->m_OpenDoorEvent * 0.007874016f)) {
            extra.a = 0;
            ImpactEvent_Call(actor, actor, 6, actor->header.m_vPos, extra);
            CScene__SetInstanceSavedBit(&gScene, actor, 1);
        }
    }
    if ((info->m_dwFlags & 1) && state->active != 0) {
        t = info->m_OpenDuration;
        if (t == 0.0f || state->duration < t) {
            if (state->duration == 0.0f) {
                state->timer = 0.0f;
            }
            state->duration = t;
        }
    }
    if (state->duration != 0.0f && state->duration <= state->timer) {
        CAI__SetMode(actor, state, 1);
    }
}
#endif
