/* FAKEMATCH: identical if/else arms in fewerActorsThan's counting loop (as in MotherBoss_FewerActorsThan,
 * src/us/code/func_0040C4F4.c, which says why and what was tried). */
#include "common.h"
#include "structs/pick.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"

/* Picks the next attack anim from a weighted table: close to the target 4 (then 3 after a 4), otherwise 5
 * when fewer than 3 actors are in the gScene_m_InstancePool_m_ActiveList list, else 6. */

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern s16 WeightedTable_PickStatic(Pick *table);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 state);

/* MotherBoss_GetTargetDist */
static __inline__ f32 getTargetDist(CGameObjectInstance *obj, CListedInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    AI_UpdateTargets(obj, &obj->m_AI, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = (CListedInstance *) t.m_Path.m_pTarget;
    }
    return dist;
}

/* MotherBoss_FewerActorsThan */
static __inline__ s32 fewerActorsThan(s32 max) {
    CListedInstance *actor;
    s32 count = 0;

    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        /* both arms are merged by cross-jumping; the extra block gives the ROM's register allocation */
        if (actor->m_pNext != NULL) {
            count++;
        } else {
            count++;
        }
    }
    return count < max;
}

void MotherBoss_Phase0ChooseAttack(CGameObjectInstance *obj, CAI *ai) {
    Pick picks[8];
    CListedInstance *found;
    MotherBossState *state = &gMotherBoss;
    s32 n;
    f32 dist;

    n = 0;
    dist = getTargetDist(obj, &found);
    if (found != NULL) {
        if (dist < 1228.7999f) {
            picks[n].id = 4;
            picks[n++].weight = 6;
            if (state->lastAnim == 4) {
                picks[n].id = 3;
                picks[n++].weight = 6;
            }
        } else {
            state->unk668 = 0;
            if (fewerActorsThan(3)) {
                picks[n].id = 5;
                picks[n++].weight = 5;
            } else {
                picks[n].id = 6;
                picks[n++].weight = 5;
            }
        }
        picks[n].id = -1;
        state->lastAnim = WeightedTable_PickStatic(picks);
        state->counter = 0;
        CAI__SetMode(obj, ai, state->lastAnim);
    }
}
