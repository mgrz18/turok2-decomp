#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"
#include "protos/enemy.h"

/* Flying AI steering, scaled by speed: like EnemyFly_SteerToPath, but the pitch and
 * roll ease faster when they have to swing to the other side. */

f32 Math_StepClamped(f32 value, f32 target, f32 step);
f32 Math_Approach(f32 cur, f32 target, f32 rate);

void EnemyFly_SteerToPathScaled(CGameObjectInstance *self, CAI *ai, CAITargets *steer, CAIFrameMotion *limits, f32 speed, f32 turnRate) {
    f32 v;
    f32 max;
    f32 pitch;
    f32 roll;
    f32 pitchSide;
    f32 rollSide;

    AI_TurnClamped(self, ai, turnRate * 0.13962635f, steer->m_Path.m_Angle);
    if (EnemyAI_GetBehaviorIndex(self, ai) == 1) {
        v = Math_StepClamped(self->m_vVelocity.y, steer->m_Path.m_vDelta.y, limits->m_Dist * (speed * 8.0f));
        self->m_vVelocity.y = v;
        max = speed * 102.399994f;
        if (v > max) {
            self->m_vVelocity.y = max;
        } else if (v < -max) {
            self->m_vVelocity.y = -max;
        }
        pitch = self->m_vVelocity.y * 0.008726647f;
        if (pitch > 0.436332345f) {
            pitch = 0.436332345f;
        } else if (pitch < -0.436332345f) {
            pitch = -0.436332345f;
        }
        pitchSide = 1.0f;
        if (pitch < 0.0f) {
            pitchSide = -1.0f;
        }
        if (pitchSide != (ai->m_Enemy.m_Pitch < 0.0f ? -1.0f : 1.0f)) {
            ai->m_Enemy.m_Pitch = Math_Approach(ai->m_Enemy.m_Pitch, pitch, 0.17453295f);
        } else {
            ai->m_Enemy.m_Pitch = Math_Approach(ai->m_Enemy.m_Pitch, pitch, 0.087266475f);
        }
        roll = steer->m_Path.m_Angle * 0.75f;
        if (roll > 0.436332345f) {
            roll = 0.436332345f;
        } else if (roll < -0.436332345f) {
            roll = -0.436332345f;
        }
        rollSide = 1.0f;
        if (roll < 0.0f) {
            rollSide = -1.0f;
        }
        if (rollSide != (ai->m_Enemy.m_Roll < 0.0f ? -1.0f : 1.0f)) {
            ai->m_Enemy.m_Roll = Math_Approach(ai->m_Enemy.m_Roll, roll, 0.3490659f);
        } else {
            ai->m_Enemy.m_Roll = Math_Approach(ai->m_Enemy.m_Roll, roll, 0.17453295f);
        }
    }
}

#include "rw/abu_protos.h"

f32 Random_Blend(f32 arg0, f32 arg1);
void EnemyRam_Collision(CGameObjectInstance *self, CAI *ai);

void EnemyRam_Setup(sptr arg0, CAI *arg1) {
    f32 temp_f20;
    f32 temp_f21;

    temp_f20 = -40.96f;
    temp_f21 = 40.96f;
    arg1->m_pfCollision = (void *) EnemyRam_Collision;
    arg1->m_vTargetOffset.x = Random_Blend(temp_f20, temp_f21);
    arg1->m_vTargetOffset.y = Random_Blend(temp_f20, temp_f21);
    arg1->m_vTargetOffset.z = Random_Blend(temp_f20, temp_f21);
}

#include "globals/engine.h"

/* How this actor uses CAI's m_VarBuffer: two angles turned toward 0. */
typedef struct {
    /* 0x00 */ f32 angle0;
    /* 0x04 */ f32 angle1;
} AITurnVars;
f32 Math_DecayToZero(f32, f32);

void EnemyRam_Advance(sptr arg0, CAI *arg1) {
    f32 temp_f0;
    f32 temp_f20;
    s8 temp_v1;

    arg1->m_vLeashPos.y = (f32) arg1->m_vInitialPos.y;
    if ((EnemyAI_GetBehaviorIndex(arg0, (sptr) arg1) != 1) || ((temp_v1 = arg1->m_Mode, (temp_v1 != 4)) && (temp_v1 != 0xE))) {
        temp_f20 = 0.17453295f;
        ((AITurnVars *) arg1->m_VarBuffer)->angle1 = Math_DecayToZero(((AITurnVars *) arg1->m_VarBuffer)->angle1, temp_f20);
        ((AITurnVars *) arg1->m_VarBuffer)->angle0 = Math_DecayToZero(((AITurnVars *) arg1->m_VarBuffer)->angle0, temp_f20);
    }
    if (arg1->m_Action == 0) {
        temp_f0 = arg1->m_ActionTime - gFrameIncrement;
        arg1->m_ActionTime = temp_f0;
        if (temp_f0 < 0.0f) {
            arg1->m_Action = 1;
        }
    }
}

void EnemyRam_MoveEnter(sptr arg0, CAI *ai) {
    ai->m_Action = 0;
    ai->m_ActionTime = AI_LerpByAggression(arg0, ai, 90.0f, 30.0f);
}

#ifdef TARGET_PORT
s16 EnemyAI_PickAttack(CGameObjectInstance *self, CAI *ai, CAITargets *t); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
#else
s32 EnemyAI_PickAttack(CGameObjectInstance *self, CAI *ai, CAITargets *t);
#endif

void EnemyRam_MoveUpdate(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    s32 next;

    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_ComputeFrameMotion(self, ai, &steer);
    t.m_Path.m_vDelta.y = t.m_Sight.m_vDelta.y = ai->m_vLeashPos.y - self->header.m_vPos.y;
    if (ai->m_Action == 1) {
        EnemyFly_SteerToPathScaled(self, ai, &t, &steer, 2.0f, 2.0f);
    } else {
        EnemyFly_SteerToPathScaled(self, ai, &t, &steer, 1.0f, 1.0f);
    }
    if (t.m_Sight.m_pTarget != 0 && (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle : t.m_Sight.m_Angle) <= 1.5707965f) {
        next = EnemyAI_PickAttack(self, ai, &t);
        if (next != 0) {
            ai->m_ModeLastFrame = -1;
            CAI__SetMode(self, ai, next);
        }
    }
}

void EnemyRam_CombatUpdate(void *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    f32 dist;

    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_ComputeFrameMotion(self, ai, &steer);
    dist = t.m_Path.m_FlatDist;
    if (!(dist >= 1024.0f)) {
        dist = 1024.0f;
    }
    EnemyFly_SteerToPathScaled(self, ai, &t, &steer, 6.0f - dist * (1.0f / 1024) * 3.0f, 2.0f);
    if ((t.m_Sight.m_pTarget != 0 && (t.m_Sight.m_vDelta.y < 0.0f ? -t.m_Sight.m_vDelta.y : t.m_Sight.m_vDelta.y) < 51.199997f) || ai->m_ModeTime > 90.0f) {
        CAI__SetMode(self, ai, 4);
    }
}

#include "structs/collision.h"
#include "globals/collision.h"

void DamageEvent_Set(DamageEvent *event, CGameObjectInstance *source, s32 damage, f32 radius, s32 flags, void *shield);

void EnemyRam_Collision(CGameObjectInstance *self, CAI *ai) {
    DamageEvent event;
    CCollisionResults *ps;

    if (ai->m_Health != 0 && ai->m_Mode == 0xE) {
        ps = &gCollisionResults;
        if (ps->m_pInstance != NULL && (ps->m_pInstance == ai->m_pSightTarget || ps->m_pInstance == ai->m_pPathTarget)) {
            DamageEvent_Set(&event, self, 0x500, 25.599998f, 0x100000, NULL);
            CAI__CallHurt(ps->m_pInstance, ((u8 *)&ps->m_pInstance->m_AI), &event);
            CAI__SetMode(self, ai, 4);
        }
    }
}

f32 func_002974C0(f32 x);
f32 Random_Blend(f32 lo, f32 hi);

void EnemySwim_Advance(CGameObjectInstance *self, CAI *ai) {
    f32 pitch;
    f32 base;
    f32 c;

    pitch = -self->m_vVelocity.y * 0.030543264f;
    if (pitch > 1.308997f) {
        pitch = 1.308997f;
    } else if (pitch < -1.308997f) {
        pitch = -1.308997f;
    }
    ai->m_Enemy.m_Pitch = Math_Approach(ai->m_Enemy.m_Pitch, pitch, 0.06981318f);

    if (ai->m_Health != 0) {
        if (ai->m_Mode == 27) {
            c = func_002974C0(ai->m_ModeTime * 0.31415930f);
            base = ai->m_Enemy.m_BaseScale;
            base += (c + 1.0f) * ((base * 1.25f - base) * 0.5f);
            self->m_vScale.x = Math_Approach(self->m_vScale.x, base * 3.0f * 2.0f, 0.1f);
            self->m_vScale.y = Math_Approach(self->m_vScale.y, base * 1.25f * 2.0f, 0.1f);
            self->m_vScale.z = Math_Approach(self->m_vScale.z, base * 1.25f * 2.0f, 0.1f);
        } else {
            self->m_vScale.x = Math_Approach(self->m_vScale.x, ai->m_Enemy.m_BaseScale, 0.1f);
            self->m_vScale.y = Math_Approach(self->m_vScale.y, ai->m_Enemy.m_BaseScale, 0.1f);
            self->m_vScale.z = Math_Approach(self->m_vScale.z, ai->m_Enemy.m_BaseScale, 0.1f);
        }
    }

    ai->m_Enemy.m_WanderTimer -= gFrameIncrement;
    if (ai->m_Enemy.m_WanderTimer <= 0.0f) {
        ai->m_Enemy.m_WanderTimer = Random_Blend(60.0f, 120.0f);
        ai->m_vTargetOffset.x = Random_Blend(-51.199997f, 51.199997f);
        ai->m_vTargetOffset.y = Random_Blend(-51.199997f, 51.199997f);
        ai->m_vTargetOffset.z = Random_Blend(-51.199997f, 51.199997f);
    }
}

#include "protos/game.h"

f32 GameObject_GetWaterHeight(CGameObjectInstance *self);
f32 Math_StepClamped(f32 value, f32 target, f32 rate);

void EnemySwim_MoveUpdate(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    CEnemyIntelligence *params;
    CAITargets *tp;
    f32 speed;
    f32 turn;
    f32 y;
    f32 lo;
    f32 hi;

    params = &self->header.m_pIntelligence->m_Enemy;
    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_ComputeFrameMotion(self, ai, &steer);
    if (t.m_Path.m_Kind == 2 && t.m_Path.m_pTarget->m_AI.m_Mode == 6) {
        speed = 4.0f;
        turn = 0.20943953f;
    } else if (t.m_Path.m_Kind == 0) {
        speed = 1.5f;
        turn = 0.104719765f;
    } else {
        speed = (func_002974C0(ai->m_ModeTime * 0.052359883f) + 1.0f) * 0.25f + 1.0f;
        turn = (func_002974C0(ai->m_ModeTime * 2.0f * 0.017453294f) + 1.0f) * 0.043633237f + 0.017453294f;
    }
    ai->m_Enemy.m_TurnRate = Math_Approach(ai->m_Enemy.m_TurnRate, turn, 0.25f);
    ai->m_AnimSpeed = Math_Approach(ai->m_AnimSpeed, speed, 0.25f);

    lo = GameObject_GetRegionFloorHeight(self, 0);
    hi = GameObject_GetWaterHeight(self);
    y = ai->m_vInitialPos.y + func_002974C0(ai->m_ModeTime * 0.0017453295f) * 102.399994f;
    if (y < lo) {
        y += 102.399994f;
    }
    if (hi < y) {
        y -= 102.399994f;
    }
    if (y < lo) {
        y = lo;
    } else if (hi < y) {
        y = hi;
    }
    ai->m_vLeashPos.y = Math_Approach(ai->m_vLeashPos.y, y, 5.12f);

    tp = &t;
    self->m_vVelocity.y = Math_StepClamped(self->m_vVelocity.y, tp->m_Path.m_vDelta.y, steer.m_Dist * 2.0f);
    if (self->m_vVelocity.y > 81.92f) {
        self->m_vVelocity.y = 81.92f;
    } else if (self->m_vVelocity.y < -81.92f) {
        self->m_vVelocity.y = -81.92f;
    }
    AI_TurnClamped(self, ai, ai->m_Enemy.m_TurnRate, tp->m_Path.m_Angle);

    if (ai->m_ModeTime > 15.0f) {
        if (params->m_UnderwaterBehavior != 5) {
            if (t.m_Path.m_Kind == 0 && t.m_Path.m_Dist < 81.92f
                && (t.m_Path.m_Angle < 0.0f ? -t.m_Path.m_Angle : t.m_Path.m_Angle) < 0.26179942f) {
                CAI__SetMode(self, ai, 6);
            }
        } else {
            if (t.m_Path.m_Kind == 0 && t.m_Path.m_Dist < params->m_AttackDartRadius) {
                CAI__SetMode(self, ai, 16);
            }
        }
    }
}

f32 func_002982F0(f32 x);

void EnemySwim_FleeEnter(CGameObjectInstance *self, CAI *ai) {
    CEnemyIntelligence *params;
    CGameObjectInstance *target;
    f32 dist;
    f32 angle;

    params = &self->header.m_pIntelligence->m_Enemy;
    target = ai->m_pPathTarget;
    if (target != NULL && params->m_UnderwaterBehavior != 5) {
        dist = Random_Blend(-102.399994f, 102.399994f) + 512.0f;
        angle = target->m_RotY + 3.1415927f + Random_Blend(-0.78539824f, 0.78539824f);
        ai->m_vLeashPos.x = target->header.m_vPos.x + dist * func_002982F0(angle);
        ai->m_vInitialPos.y = (ai->m_vLeashPos.y + target->header.m_vPos.y) * 0.5f + Random_Blend(-51.199997f, 51.199997f);
        ai->m_vLeashPos.z = target->header.m_vPos.z + dist * func_002974C0(angle);
    }
    if (params->m_UnderwaterBehavior == 4 && ai->m_ModeBefore != 27) {
        CAI__SetMode(self, ai, 27);
    } else if (params->m_UnderwaterBehavior == 3 && ai->m_ModeBefore != 10) {
        CAI__SetMode(self, ai, 10);
    }
}

void EnemySwim_Collision(CGameObjectInstance *self, CAI *ai) {
    DamageEvent event;

    if (gCollisionResults.m_pWallRegion != 0) {
        ai->m_vLeashPos = ai->m_vInitialPos;
    }
    if (gCollisionResults.m_pInstance != NULL && ai->m_Mode == 0x10 && gCollisionResults.m_pInstance == ai->m_pPathTarget) {
        DamageEvent_Set(&event, self, 0x200, 25.599998f, 0x100000, NULL);
        CAI__CallHurt(gCollisionResults.m_pInstance, ((u8 *)&gCollisionResults.m_pInstance->m_AI), &event);
        CAI__SetMode(self, ai, 6);
    }
}

void EnemySwim_Setup(CGameObjectInstance *self, CAI *ai) {
    ai->m_pfCollision = (void (*)(CGameObjectInstance *, void *)) EnemySwim_Collision;
    ai->m_ModeTime = Random_Blend(0.0f, 62.83186f);
    ai->m_Enemy.m_Pitch = 0.0f;
    ai->m_Enemy.m_TurnRate = 0.017453294f;
    ai->m_Enemy.m_BaseScale = self->m_vScale.x;
    ai->m_Enemy.m_WanderTimer = 0.0f;
}

void EnemySwim_FleeUpdate(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    CAITargets *tp;

    ai->m_Enemy.m_TurnRate = Math_Approach(ai->m_Enemy.m_TurnRate, 0.20943953f, 0.25f);
    ai->m_AnimSpeed = Math_Approach(ai->m_AnimSpeed, 6.0f, 0.25f);
    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_ComputeFrameMotion(self, ai, &steer);

    tp = &t;
    self->m_vVelocity.y = Math_StepClamped(self->m_vVelocity.y, tp->m_Path.m_vDelta.y, steer.m_Dist * 2.0f);
    if (self->m_vVelocity.y > 81.92f) {
        self->m_vVelocity.y = 81.92f;
    } else if (self->m_vVelocity.y < -81.92f) {
        self->m_vVelocity.y = -81.92f;
    }
    AI_TurnClamped(self, ai, ai->m_Enemy.m_TurnRate, tp->m_Path.m_Angle);

    if (ai->m_ModeTime > 15.0f && (t.m_Path.m_FlatDist < 81.92f || ai->m_ModeTime > 90.0f)) {
        CAI__SetMode(self, ai, 4);
    }
}

extern f32 Random_Blend(f32, f32);

void EnemySwim_IdleEnter(sptr arg0, CAI *arg1) {
    arg1->m_ConditionTimer = Random_Blend(0.0f, 3.0e1f);
}

void EnemySwim_IdleUpdate(CGameObjectInstance *self, CAI *ai) {
    ai->m_AnimSpeed = Math_Approach(ai->m_AnimSpeed, 1.0f, 0.2f);
    self->m_vVelocity.y = Math_Approach(self->m_vVelocity.y, 0.0f, 1.024f);
    if (ai->m_ModeTime >= ai->m_ConditionTimer) {
        CAI__SetMode(self, ai, 6);
    }
}

void EnemySwim_PuffUpEnter(CGameObjectInstance *self, CAI *ai) {
    self->m_dwFlags &= ~0x20000;
    ai->m_ConditionTimer = Random_Blend(30.0f, 60.0f);
}

void EnemySwim_PuffUpUpdate(CGameObjectInstance *self, CAI *ai) {
    ai->m_AnimSpeed = Math_Approach(ai->m_AnimSpeed, 0.25f, 0.2f);
    self->m_vVelocity.y = Math_Approach(self->m_vVelocity.y, 0.0f, 1.024f);
    if (ai->m_ModeTime >= ai->m_ConditionTimer) {
        self->m_dwFlags |= 0x20000;
        CAI__SetMode(self, ai, 6);
    }
}

void EnemySwim_DartUpdate(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    CAITargets *tp;

    ai->m_vTargetOffset.x = 0.0f;
    ai->m_vTargetOffset.z = 0.0f;
    ai->m_AnimSpeed = Math_Approach(ai->m_AnimSpeed, 6.0f, 0.2f);
    ai->m_Enemy.m_TurnRate = Math_Approach(ai->m_Enemy.m_TurnRate, 0.20943953f, 0.5f);
    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_ComputeFrameMotion(self, ai, &steer);

    tp = &t;
    self->m_vVelocity.y = Math_StepClamped(self->m_vVelocity.y, tp->m_Path.m_vDelta.y, steer.m_Dist * 2.0f);
    if (self->m_vVelocity.y > 81.92f) {
        self->m_vVelocity.y = 81.92f;
    } else if (self->m_vVelocity.y < -81.92f) {
        self->m_vVelocity.y = -81.92f;
    }
    AI_TurnClamped(self, ai, ai->m_Enemy.m_TurnRate, tp->m_Path.m_Angle);
}

typedef struct {
    /* 0x00 */ u8 pad0[0x4C];
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ u8 pad50[0x60 - 0x50];
    /* 0x60 */ f32 unk60;
} Arg2;
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 unk18;
} Arg3;
f32 Math_StepClamped(f32 value, f32 delta, f32 limit);

void EnemySwim_SteerToPath(CGameObjectInstance *self, CAI *ai, Arg2 *a2, Arg3 *a3) {
    self->m_vVelocity.y = Math_StepClamped(self->m_vVelocity.y, a2->unk60, a3->unk18 * 2.0f);
    if (self->m_vVelocity.y > 81.92f) {
        self->m_vVelocity.y = 81.92f;
    } else if (self->m_vVelocity.y < -81.92f) {
        self->m_vVelocity.y = -81.92f;
    }
    AI_TurnClamped(self, ai, ai->m_Enemy.m_TurnRate, a2->unk4C);
}
