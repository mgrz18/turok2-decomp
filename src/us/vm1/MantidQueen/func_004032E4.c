#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

void AI_UpdateTargets(void *, void *, CAITargets *, s32, s32);

void MantidQueen_UpdateMode15(sptr a0, sptr a1) {
    CAITargets buf;

    AI_UpdateTargets((void *) a0, (void *) a1, &buf, 1, 1);
    AI_TurnClamped((void *) a0, a1, 0.0174532961f, buf.m_Path.m_Angle);
}
