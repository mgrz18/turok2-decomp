#include "common.h"
#include "globals/boss.h"

#include "structs/game_object.h"
#include "protos/ai.h"

void AI_UpdateTargets(void *, void *, CAITargets *, s32, s32);
s32 AI_IsAimedAt(void *, void *, sptr, f32);

static __inline__ s32 func_00403774_inline(sptr a0, CAI *item, sptr a2) {
    s32 result;

    if (gMantidQueen.blink == 0) {
        result = 0;
    } else {
        result = AI_IsAimedAt((void *) a0, item, a2, 0.785398245f);
    }
    return result;
}

void MantidQueen_UpdateTurn(sptr a0, CAI *item) {
    CAITargets info;
    s32 side;

    AI_UpdateTargets((void *) a0, item, &info, 1, 1);
    AI_TurnClamped((void *) a0, (sptr) item, 0.17453295f, info.m_Path.m_Angle);
    side = func_00403774_inline(a0, item, (sptr) info.m_Sight.m_pTarget);
    if (side == -1) {
        CAI__SetMode((void *) a0, item, 11);
    } else if (side == 1) {
        CAI__SetMode((void *) a0, item, 12);
    } else if (item->m_AnimRepeatedCount >= 4) {
        CAI__SetMode((void *) a0, item, 8);
    }
}
