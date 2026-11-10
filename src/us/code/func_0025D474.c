#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "protos/ai.h"

/* AI chase update: turns toward the target and runs or walks at it; flying
 * modes (1, 2) also climb toward its height and bank into the turn. In range,
 * it attacks (path node 4 / special 7) or lets EnemyAI_PickAttack pick a melee. */

typedef struct {
    /* 0x00 */ CVector3 d;
    /* 0x0C */ CVector3 n;
    /* 0x18 */ f32 len;
    /* 0x1C */ CVector3 e;
    /* 0x28 */ CVector3 e2;
    /* 0x34 */ f32 elen;
    /* 0x38 */ f32 dy;
    /* 0x3C */ u8 pad3C[0x40 - 0x3C];
} Dir;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u16 flags;
} PathNode;

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

extern void AI_UpdateTargets(CGameObjectInstance *actor, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void AI_ComputeFrameMotion(CGameObjectInstance *actor, CAI *ai, Dir *dir);
extern f32 Math_StepClamped(f32 value, f32 delta, f32 limit);
extern f32 Math_Approach(f32 value, f32 target, f32 rate);
#ifdef TARGET_PORT
extern s16 EnemyAI_PickAttack(CGameObjectInstance *actor, CAI *ai, CAITargets *t); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
#else
extern s32 EnemyAI_PickAttack(CGameObjectInstance *actor, CAI *ai, CAITargets *t);
#endif
extern void EnemyAI_TryDodge(CGameObjectInstance *actor, CAI *ai);
extern void EnemyAI_IdleSoundCallback(CGameObjectInstance *actor, CAI *ai, CAITargets *t);
extern PathNode *CPathTrack__GetCurrentPoint(void *path);
extern void CPathTrack__Advance(void *path);

void EnemyMode_Move_Update(CGameObjectInstance *actor, CAI *ai) {
    CAITargets t;
    Dir dir;
    s32 runAnim;
    s32 walkAnim;
    s32 dodge;
    s32 mode;
    s32 anim;
    f32 maxSpeed;
    f32 angle;
    f32 turn;
    f32 range;
    f32 bank;

    actor->m_dwFlags |= 0x20000;
    actor->m_dwFlags |= 0x20000000;
    AI_UpdateTargets(actor, ai, &t, 1, 1);
    runAnim = 4;
    if (t.m_Path.m_Kind == 1 && actor->header.m_pIntelligence->m_Enemy.m_wFleeAnimFlags) {
        dodge = 0;
        runAnim = 6;
    } else {
        dodge = 0;
    }
    walkAnim = 4;
    if (ai->m_Action == 0) {
        walkAnim = 3;
    }
    AI_ComputeFrameMotion(actor, ai, &dir);
    mode = ai->m_Enemy.m_BehaviorIndex;
    if (mode == 1 || mode == 2) {
        maxSpeed = 81.92f;
        if (mode == 1) {
            maxSpeed = 163.84f;
        }
        if (t.m_Path.m_Dist > 10.24f) {
            turn = 0.261799425f;
            if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
                turn = 1.0471977f;
            }
            AI_TurnClamped(actor, ai, turn, t.m_Path.m_Angle);
        }
        if (t.m_Path.m_Kind == 4) {
            CAI__SetMode(actor, ai, walkAnim);
        } else {
            CAI__SetMode(actor, ai, runAnim);
        }
        actor->m_vVelocity.y = Math_StepClamped(actor->m_vVelocity.y, t.m_Path.m_vDelta.y, dir.len * 16.0f);
        if (maxSpeed < actor->m_vVelocity.y) {
            actor->m_vVelocity.y = maxSpeed;
        } else if (actor->m_vVelocity.y < -maxSpeed) {
            actor->m_vVelocity.y = -maxSpeed;
        }
        if (mode == 2) {
            bank = -actor->m_vVelocity.y * 0.030543264f;
            if (bank > 0.43633235f) {
                bank = 0.43633235f;
            } else if (bank < -0.43633235f) {
                bank = -0.43633235f;
            }
            ai->m_Enemy.m_Pitch = Math_Approach(ai->m_Enemy.m_Pitch, bank, 0.069813177f);
        }
    } else if (!(ai->m_ModeFlags & 0x400)) {
        angle = 0.261799425f;
        if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
            angle = 1.0471977f;
        }
        if (t.m_Path.m_Dist < 204.79999f) {
            angle *= 3.0f;
        }
        if (ai->m_Mode == 3) {
            angle *= 0.5f;
        }
        AI_TurnClamped(actor, ai, angle, t.m_Path.m_Angle);
    } else {
        turn = 0.261799425f;
        if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
            turn = 1.0471977f;
        }
        AI_TurnClamped(actor, ai, turn, t.m_Sight.m_Angle);
    }
    if (t.m_Sight.m_pTarget) {
        angle = 0.261799425f;
        if (actor->m_nTypeFlag == 0x259) {
            angle = 1.5707965f;
        }
        if (ABS(t.m_Sight.m_Angle) <= angle) {
            anim = EnemyAI_PickAttack(actor, ai, &t);
            if (anim != 0) {
                ai->m_ModeLastFrame = -1;
                CAI__SetMode(actor, ai, anim);
                return;
            }
        }
    }
    switch (t.m_Path.m_Kind) {
    case 7:
        range = 30.72f;
        break;
    case 4:
        range = 61.44f;
        if (walkAnim == 3) {
            range = 30.72f;
        }
        break;
    default:
        range = 102.399994f;
        break;
    }
    if (ai->m_ModeFlags & 0x400) {
        range *= 2.0f;
    }
    if (ai->m_Mode != 5 || ai->m_AnimRepeatedCount != 0) {
        if (t.m_Path.m_Dist <= range) {
            switch (t.m_Path.m_Kind) {
            case 4:
                if (CPathTrack__GetCurrentPoint(&ai->m_PathTrack)->flags & 1) {
                    CAI__SetMode(actor, ai, 0xB);
                } else {
                    CPathTrack__Advance(&ai->m_PathTrack);
                }
                break;
            case 7:
                CAI__SetMode(actor, ai, 0xD);
                if (actor->m_dwFlags & 0x400) {
                    ai->m_vDesiredPos.x += t.m_Path.m_vDelta.x * 0.5f;
                    ai->m_vDesiredPos.z += t.m_Path.m_vDelta.z * 0.5f;
                }
                break;
            default:
                CAI__SetMode(actor, ai, 0xA);
                break;
            }
        } else if (t.m_Path.m_Kind == 4) {
            CAI__SetMode(actor, ai, walkAnim);
        } else if (dodge && ABS(t.m_Path.m_Angle) > 2.3561947f) {
            CAI__SetMode(actor, ai, 5);
        } else {
            CAI__SetMode(actor, ai, runAnim);
        }
    }
    EnemyAI_TryDodge(actor, ai);
    EnemyAI_IdleSoundCallback(actor, ai, &t);
}
