#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/enemy_ai.h"

/* Picks the animation for the AI's current location: one candidate from the
 * location's primary flags and one from its secondary flags, each from the
 * table for the current mode; with both, a coin toss decides. */


extern s32 Random_Next(void);

s32 EnemyAnim_InteractiveIdle(CGameObjectInstance *actor, CAI *ai) {
    CPathPoint *loc = CPathTrack__GetCurrentPoint(&ai->m_PathTrack);
    s32 animA = -1;
    s32 mode = ai->m_Enemy.m_BehaviorIndex;
    s32 animB = -1;

    if (loc->m_AnimFlagsA != 0) {
        switch (mode) {
        case 0:
            animA = AI_PickAnimFromTable(actor, (sptr) ai, sEnemyAnimIdle_AF6F0, loc->m_AnimFlagsA);
            break;
        case 1:
            animA = AI_PickAnimFromTable(actor, (sptr) ai, sEnemyAnimIdle_AF720, loc->m_AnimFlagsA);
            break;
        case 2:
            animA = AI_PickAnimFromTable(actor, (sptr) ai, sEnemyAnimIdle_AF750, loc->m_AnimFlagsA);
            break;
        }
    }
    if (loc->m_AnimFlagsB != 0) {
        switch (mode) {
        case 0:
            animB = AI_PickAnimFromTable(actor, (sptr) ai, sEnemyAnimIdle_AF780, loc->m_AnimFlagsB);
            break;
        case 1:
            animB = AI_PickAnimFromTable(actor, (sptr) ai, sEnemyAnimIdle_AF7B0, loc->m_AnimFlagsB);
            break;
        case 2:
            animB = AI_PickAnimFromTable(actor, (sptr) ai, sEnemyAnimIdle_AF7E0, loc->m_AnimFlagsB);
            break;
        }
    }
    if (animA != -1 && animB != -1) {
        if (Random_Next() % 1000 >= 501) {
            return animA;
        }
        return animB;
    }
    if (animA != -1) {
        return animA;
    }
    if (animB != -1) {
        return animB;
    }
    return 0;
}
