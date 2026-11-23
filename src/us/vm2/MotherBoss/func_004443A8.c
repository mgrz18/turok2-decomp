/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B3A8.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

/* AI state: turn toward the target (15 degrees per step), then go to anim state 0xC. */

extern void AI_UpdateTargets(void *obj, void *ai, CAITargets *t, s32 f1, s32 f2);

void MotherBoss_Mode11_TrackTarget_2(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0xC);
    }
}
