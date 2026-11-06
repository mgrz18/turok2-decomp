#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x88 - 0xC];
} TargetInfo; /* size 0x88 */

void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, TargetInfo *info, s32 a, s32 b);
void AI_TurnClamped(CGameObjectInstance *obj, CAI *ai, f32 angle, s32 unk);

/* AI callback: turns toward the target, 60 degrees at a time in modes 1-2,
 * else 15. */
void EnemyAI_TurnToTarget(CGameObjectInstance *obj, CAI *ai) {
    TargetInfo info;
    f32 angle;

    AI_UpdateTargets(obj, ai, &info, 0, 1);
    angle = 0.261799425f; /* 15 deg */
    if ((u8) ai->m_ModeBefore == 1 || (u8) ai->m_ModeBefore == 2) {
        angle = 1.0471977f; /* 60 deg */
    }
    /* FAKEMATCH: the identical branches are merged by cross-jumping; the extra
     * use of obj only gives it the register priority it has in the ROM ($s0
     * for obj, $s1 for ai). */
    if (obj->header.m_pIntelligence != NULL) {
        AI_TurnClamped(obj, ai, angle, info.unk8);
    } else {
        AI_TurnClamped(obj, ai, angle, info.unk8);
    }
}
