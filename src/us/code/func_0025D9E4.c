#include "common.h"
#include "structs/game_object.h"

/* Picks the animation for the AI's current location: one candidate from the
 * location's primary flags and one from its secondary flags, each from the
 * table for the current mode; with both, a coin toss decides. */

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 anim;
} AnimEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x16];
    /* 0x16 */ u16 flagsA;
    /* 0x18 */ u16 flagsB;
} Location;

extern AnimEntry sEnemyAnimIdle_AF6F0[];
extern AnimEntry sEnemyAnimIdle_AF720[];
extern AnimEntry sEnemyAnimIdle_AF750[];
extern AnimEntry sEnemyAnimIdle_AF780[];
extern AnimEntry sEnemyAnimIdle_AF7B0[];
extern AnimEntry sEnemyAnimIdle_AF7E0[];

extern Location *CPathTrack__GetCurrentPoint(s8 *location);
extern s32 AI_PickAnimFromTable(void *actor, CAI *ai, AnimEntry *table, s32 flags);
extern s32 Random_Next(void);

s32 EnemyAnim_InteractiveIdle(void *actor, CAI *ai) {
    Location *loc = CPathTrack__GetCurrentPoint(&ai->m_PathTrack.m_Mode);
    s32 animA = -1;
    s32 mode = ai->m_Enemy.m_BehaviorIndex;
    s32 animB = -1;

    if (loc->flagsA != 0) {
        switch (mode) {
        case 0:
            animA = AI_PickAnimFromTable(actor, ai, sEnemyAnimIdle_AF6F0, loc->flagsA);
            break;
        case 1:
            animA = AI_PickAnimFromTable(actor, ai, sEnemyAnimIdle_AF720, loc->flagsA);
            break;
        case 2:
            animA = AI_PickAnimFromTable(actor, ai, sEnemyAnimIdle_AF750, loc->flagsA);
            break;
        }
    }
    if (loc->flagsB != 0) {
        switch (mode) {
        case 0:
            animB = AI_PickAnimFromTable(actor, ai, sEnemyAnimIdle_AF780, loc->flagsB);
            break;
        case 1:
            animB = AI_PickAnimFromTable(actor, ai, sEnemyAnimIdle_AF7B0, loc->flagsB);
            break;
        case 2:
            animB = AI_PickAnimFromTable(actor, ai, sEnemyAnimIdle_AF7E0, loc->flagsB);
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
