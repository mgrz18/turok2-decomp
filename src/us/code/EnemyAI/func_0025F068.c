#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy_ai.h"

void EnemyMode_InteractiveIdle_Update(CGameObjectInstance *obj, CAI *ai) {
    CAITargets info;
    f32 angle;

    AI_UpdateTargets(obj, ai, &info, 0, 1);
    angle = 0.2617994249f; /* 15 deg */
    if ((u8) ai->m_ModeBefore == 1 || (u8) ai->m_ModeBefore == 2) {
        angle = 1.0471977f; /* 60 deg */
    }
    AI_TurnClamped(obj, ai, angle, info.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount != 0 || ai->m_Action == 1) {
        CPathTrack__Advance(&ai->m_PathTrack);
        CAI__SetMode(obj, ai, 10);
    }
}
