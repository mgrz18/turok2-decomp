/* WRONG CUT: splat's func_0025AD88 starts two orphan words early (0x25AD88 `sw $t0, 0x20($sp)`,
 * 0x25AD8C `bnez $v0, .L0025B284`, whose delay slot is this function's first instruction).
 * The real function is AI_TurnHeadToTarget (0x2E8 bytes); this C matches it from 0x25AD90 on. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "globals/ai.h"
#include "protos/iset.h"
#include "protos/cache.h"

/* AI head tracking: turns the head bones toward the target (or the player in state 0xC). */

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 neckBone;
    /* 0x19 */ s8 headBone;
    /* 0x1A */ s8 boneCount;
} AnimObjectInfo;

extern f32 GameObject_GetCollisionHeight(CGameObjectInstance *actor); /* height */
extern f32 AI_AngleToPoint(CGameObjectInstance *actor, CVector3 point);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
extern f32 CVector3__GetPitch(CVector3 *v);
extern void Math_EaseToward(f32 *value, f32 target, f32 rate);

void AI_TurnHeadToTarget(CGameObjectInstance *actor, CAI *ai, f32 pitchOffset) {
    CVector3 from;
    CVector3 to;
    CVector3 delta;
    f32 value;
    s32 *res;
    AnimObjectInfo *info;
    s32 count;
    f32 yaw;
    f32 pitch;
    f32 n;
    CGameObjectInstance *target;

    res = CCache__Request(0, actor->m_rpObject, actor->m_rpObjectHeader, 0, 0, 0, "anim object info", 1);
    if (res == NULL) {
        return;
    }
    info = CISet__GetBlock(*res, 0);
    if (info->headBone != -1 && info->neckBone != -1 && (count = info->boneCount) != 0) {
        if ((ai->m_ModeFlags & 2) && ai->m_Health != 0 && (ai->m_pSightTarget != NULL || ai->m_Mode == 0xC)) {
            from.x = actor->header.m_vPos.x;
            from.y = actor->header.m_vPos.y + GameObject_GetCollisionHeight(actor) * 0.9f + actor->m_CollisionYOffset;
            from.z = actor->header.m_vPos.z;
            if (ai->m_Mode == 0xC) {
                to = gEnemyAlertPoint;
            } else {
                target = ai->m_pSightTarget;
                to.x = target->header.m_vPos.x;
                to.y = target->header.m_vPos.y + GameObject_GetCollisionHeight(target) * 0.9f + target->m_CollisionYOffset;
                to.z = target->header.m_vPos.z;
            }
            yaw = -AI_AngleToPoint(actor, to);
            if (yaw < -1.5707965f) {
                yaw = -1.5707965f;
            } else if (yaw > 1.5707965f) {
                yaw = 1.5707965f;
            }
            CVector3__Subtract(&delta, &to, &from);
            pitch = CVector3__GetPitch(&delta);
            if (pitch < -1.5707965f) {
                pitch = -1.5707965f;
            } else if (pitch > 1.5707965f) {
                pitch = 1.5707965f;
            }
        } else {
            pitch = yaw = 0.0f;
        }
        n = count;
        value = ai->m_HeadTurn * n;
        Math_EaseToward(&value, yaw, 0.25f);
        ai->m_HeadTurn = value / n;
        value = ai->m_HeadTilt * n;
        Math_EaseToward(&value, pitch + pitchOffset, 0.25f);
        ai->m_HeadTilt = value / n;
    }
    CCache__Unlock(0, res);
}
