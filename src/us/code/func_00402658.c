#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/boss.h"
#include "globals/boss.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
} Path;

extern s32 gMantidQueen_blink; /* local type: the field's real type (or the member path) changes the ROM's code here */

extern void AI_UpdateTargets(CGameObjectInstance *actor, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void AI_ComputeFrameMotion(CGameObjectInstance *actor, CAI *ai, Path *path);
extern void AI_TurnClamped(CGameObjectInstance *actor, CAI *ai, f32 angle, f32 dist);
extern s32 AI_IsAimedAt(CGameObjectInstance *actor, CAI *ai, s32 visible, f32 angle);
extern void CAI__SetMode(CGameObjectInstance *actor, CAI *ai, s32 anim);
extern f32 func_00298470(f32); /* sqrtf */

void MantidQueen_ChooseAttack(CGameObjectInstance *actor, CAI *ai) {
    CAITargets t;
    Path path;
    CVector3 v;
    WaveBossStep *step;
    WaveBossState *boss = &gMantidQueen;
    f32 turn;
    s32 side;
    s32 visible;

    step = boss->step;
    actor->m_dwFlags |= 0x2000;
    turn = 0.17453295f;
    if (boss->waveActive) {
        turn = 0.3490659f;
    }
    AI_UpdateTargets(actor, ai, &t, 1, 1);
    AI_ComputeFrameMotion(actor, ai, &path);
    AI_TurnClamped(actor, ai, turn, t.m_Path.m_Angle);
    visible = t.m_Sight.m_pTarget;
    if (gMantidQueen_blink == 0) {
        side = 0;
    } else {
        side = AI_IsAimedAt(actor, ai, visible, 0.785398245f);
    }
    if (side == -1) {
        CAI__SetMode(actor, ai, 11);
    } else if (side == 1) {
        CAI__SetMode(actor, ai, 12);
    } else if (boss->waveActive) {
        v.x = actor->header.m_vPos.x;
        v.y = 0.0f;
        v.z = actor->header.m_vPos.z;
        if (func_00298470(v.x * v.x + v.z * v.z) < 204.79999f) {
            CAI__SetMode(actor, ai, 4);
        }
    } else if (t.m_Path.m_pTarget && t.m_Sight.m_pTarget) {
        if (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle <= 0.52359885f : t.m_Sight.m_Angle <= 0.52359885f) {
            if (t.m_Path.m_Dist > 1228.7999f && step->unkC) {
                CAI__SetMode(actor, ai, 9);
            } else if (t.m_Path.m_Dist < 921.6f) {
                CAI__SetMode(actor, ai, 10);
            }
        }
    }
}
