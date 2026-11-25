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
#include "structs/particle_event.h"

extern void ImpactEvent_Call(CGameObjectInstance *a, CGameObjectInstance *b, s32 kind, CVector3 pos, CImpactEventArg extra);

/* The door's CAI by member (C23; round 30 dropped the unused ActionState view and its
 * animReached): the timer m_ModeTime (N64 0x40), the duration m_ConditionTimer (0x64),
 * m_AnimIndex (0xC6), m_AnimRepeatedCount (0xC7). Read at the N64 offsets on the host, the
 * "duration ran out" test was true on the first update and no door ever opened. */
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
    CImpactEventArg extra;
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

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00204A68.c, func_00204A68.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

void ImpactEvent_Call(CGameObjectInstance *a, CGameObjectInstance *b, s32 kind, CVector3 pos, CImpactEventArg extra);

static __inline__ s32 r01_check(CGameObjectInstance *actor, CAI *state, f32 scale) {
    if (actor->m_nAnims == 1) return 1;
    if (actor->m_CurrentAnimation.m_nAnimIndex != (s8)state->m_AnimIndex || actor->m_CurrentAnimation.m_nRequestedAnimIndex != (s8)state->m_AnimIndex) return 0;
    if (state->m_AnimRepeatedCount != 0 && !(actor->m_dwFlags & 0x400)) return 1;
    if (actor->m_CurrentAnimation.m_nFrames < 3 && (actor->m_dwFlags & 0x400)) {
        return (f32)actor->m_BlendLength * scale <= actor->m_BlendPercent;
    } else {
        return (f32)actor->m_CurrentAnimation.m_nFrames * scale <= actor->m_CurrentAnimation.m_AnimationFrame;
    }
}

void Door_CloseUpdate(CGameObjectInstance *actor, CAI *state) {
    s32 idx = *(s8 *)((u8 *) actor->header.m_pIntelligence + 0x1D);
    CImpactEventArg extra;
    if (idx != -1) {
        if (r01_check(actor, state, (f32)idx * 0.007874016f)) {
            extra.a = 0;
            ImpactEvent_Call(actor, actor, 7, actor->header.m_vPos, extra);
            CScene__SetInstanceSavedBit(&gScene, actor, 0);
        }
    }
}

#include "globals/ai.h"

void Door_Step(void *arg0, CAI *arg1);

void Door_Setup(sptr arg0, CAI *ai) {
    ai->m_pModeTable = gDoorModeTable;
    ai->m_pfStep = Door_Step;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80203F04_de_3.c, func_80204BD0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/world.h"

void Door_Advance(void *arg0, void *arg1) {
    extern s32 CAI__SetMode(void *, void *, s32);
    s32 different = CScene__GetInstanceSavedBit(&gScene, arg0) != 1;
    if (different == 0) {
        CAI__SetMode(arg0, arg1, 0);
    } else {
        CAI__SetMode(arg0, arg1, 1);
    }
}

/* Calls the current AI mode's callback, if any. */
/* ABI (docs/port/ABI.md section 5, abicheck indirect missing_args): MIPS calls the mode callback
 * with this function's own a0/a1 (pass-through: the actor and its AI); the host passes them, and
 * the actor stays a full pointer (the callbacks take CGameObjectInstance *). */
void Door_Step(void *arg0, CAI *arg1) {
    s32 (*cb)(void *, CAI *);

    if (arg1->m_pModeCurrent != NULL) {
        cb = (s32 (*)(void *, CAI *))arg1->m_pModeCurrent->m_pfCallback;
        if (cb != NULL) {
            cb(arg0, arg1);
        }
    }
}

#include "globals/swoosh.h"
#include "protos/game.h"

void SwooshSet_FreeOwner(void *p, CGameObjectInstance *actor);

void Door_OpenEnter(CGameObjectInstance *actor, CAI *state) {
    CImpactEventArg extra;
    extra.a = 0;
    CScene__SetInstanceSavedBit(&gScene, actor, 1);
    GameObject_FireTriggers(actor, 0x4000, actor);
    state->m_ConditionTimer = 0.0f;
    SwooshSet_FreeOwner(&gEngineApp_m_SwooshSystem, actor);
    if (gScene.m_Mode != 4) {
        ImpactEvent_Call(actor, actor, 6, actor->header.m_vPos, extra);
        actor->m_dwFlags |= 0x8000000;
    }
}

void Door_CloseEnter(CGameObjectInstance *actor) {
    CImpactEventArg extra;
    extra.a = 0;
    CScene__SetInstanceSavedBit(&gScene, actor, 0);
    GameObject_FireTriggers(actor, 0x8000, actor);
    SwooshSet_FreeOwner(&gEngineApp_m_SwooshSystem, actor);
    if (gScene.m_Mode != 4) {
        ImpactEvent_Call(actor, actor, 7, actor->header.m_vPos, extra);
        actor->m_dwFlags |= 0x8000000;
    }
}

#include "m2c_macros.h"

void Door_EventOpen(sptr arg0, sptr arg1) {
    CAI__SetMode(arg0, arg1, 0);
}

#include "structs/event.h"

void Door_EventOpenRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 1);
}

void Door_EventClose(sptr arg0, sptr arg1) {
    CAI__SetMode(arg0, arg1, 1);
}

void Door_EventCloseRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 0);
}

s32 Door_IsOpen(sptr arg0) {
    return CScene__GetInstanceSavedBit(&gScene, arg0) == 1;
}

void Door_OpenNow(CGameObjectInstance *actor) {
    CImpactEventArg extra;
    extra.a = 0;
    ImpactEvent_Call(actor, actor, 6, actor->header.m_vPos, extra);
    CScene__SetInstanceSavedBit(&gScene, actor, 1);
}

void Door_CloseNow(CGameObjectInstance *actor) {
    CImpactEventArg extra;
    extra.a = 0;
    ImpactEvent_Call(actor, actor, 7, actor->header.m_vPos, extra);
    CScene__SetInstanceSavedBit(&gScene, actor, 0);
}

/* Sets the condition timer to arg2 (0 always; otherwise only to raise it),
 * restarting the mode time when the timer was 0. */
void AI_RaiseConditionTimer(s32 arg0, CAI *arg1, f32 arg2) {
    if ((arg2 == 0.0f) || (arg1->m_ConditionTimer < arg2)) {
        if (arg1->m_ConditionTimer == 0.0f) {
            arg1->m_ModeTime = 0.0f;
        }
        arg1->m_ConditionTimer = arg2;
    }
}

/* Boundary fixed in versions/cuts.us.txt: splat's AI_AnimReached (0x12C bytes) holds two functions:
 *   0x00265954  AI_AnimReached, the animation check (0xD0 bytes), then a nop;
 *   0x00265A28  two stray words (beq $18, $16, ...; sll $2, $18, 3), never run;
 *   0x00265A30  Destructible_Setup, an init callback (0x50 bytes; its pointer is in
 *               the data as 0x00265A30).
 * The check is the same as the inlined r01_check of Door_CloseUpdate and
 * func_002651FC, here out of line. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0020486C.c, r01_check, with Turok 2's layouts. */
#include "common.h"

/* Has the actor's animation for this state played up to `scale` of its length? */
s32 AI_AnimReached(CGameObjectInstance *actor, CAI *state, f32 scale) {
    if (actor->m_nAnims == 1) return 1;
    if (actor->m_CurrentAnimation.m_nAnimIndex != (s8) state->m_AnimIndex || actor->m_CurrentAnimation.m_nRequestedAnimIndex != (s8) state->m_AnimIndex) return 0;
    if (state->m_AnimRepeatedCount != 0 && !(actor->m_dwFlags & 0x400)) return 1;
    if (actor->m_CurrentAnimation.m_nFrames < 3 && (actor->m_dwFlags & 0x400)) {
        return (f32) actor->m_BlendLength * scale <= actor->m_BlendPercent;
    } else {
        return (f32) actor->m_CurrentAnimation.m_nFrames * scale <= actor->m_CurrentAnimation.m_AnimationFrame;
    }
}

extern void Destructible_Step(CGameObjectInstance *actor, void *state);
extern void Destructible_Hurt(CGameObjectInstance *actor, void *state);
extern void TotemDefense_Init(void);
