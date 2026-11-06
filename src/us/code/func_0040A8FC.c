/* FAKEMATCH: identical if/else arms in fewerActorsThan's counting loop (as in MotherBoss_FewerActorsThan); the plain
 * loop shifts its registers by one. */
#include "common.h"
#include "structs/pick.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"

/* Picks the next attack anim from a weighted table: close to the target (or while anim 0x17 runs its first
 * 100 steps) 0x1A/0x1B; otherwise 0x18 after a long 0x17, and from afar 0x19 (when fewer than 2 actors are
 * in the gScene_m_InstancePool_m_ActiveList list), 0x1D and 0x17 weighted by the module's timer ratio. */

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

void MotherBoss_Phase2ChooseAttack(CGameObjectInstance *obj, CAI *ai) {
    Pick picks[8];
    CListedInstance *found;
    MotherBossState *state = &gMotherBoss;
    s32 n;
    s32 close;
    s32 far;
    s32 mid;
    s32 low;
    s32 veryLow;
    f32 dist;
    f32 ratio;

    n = veryLow = low = mid = far = close = 0;
    dist = getTargetDist(obj, &found);
    if (state->totalHealth != 0) {
        ratio = (f32)state->health / (f32)state->totalHealth;
    } else {
        ratio = 1.0f;
    }
    if (ratio < 0.4f) {
        low = 1;
    }
    if (ratio < 0.6f) {
        veryLow = 1;
    }
    if (found != NULL) {
        if (state->lastAnim == 0x17) {
            if (state->counter < 101) {
                close = 1;
            } else {
                picks[n].id = 0x18;
                picks[n++].weight = 5;
            }
        } else if (dist < 460.8f) {
            close = 1;
        } else {
            far = mid = 1;
        }
        if (close) {
            picks[n].id = 0x1A;
            picks[n++].weight = 5;
            picks[n].id = 0x1B;
            picks[n++].weight = 5;
        } else {
            if (far) {
                if (fewerActorsThan(2)) {
                    picks[n].id = 0x19;
                    picks[n++].weight = 4;
                }
                picks[n].id = 0x1D;
                picks[n++].weight = 6;
            }
            if (mid) {
                picks[n].id = 0x17;
                if (low) {
                    picks[n].weight = 6;
                } else if (veryLow) {
                    picks[n].weight = 9;
                } else {
                    picks[n].weight = 12;
                }
                n++;
            }
        }
        picks[n].id = -1;
        state->lastAnim = WeightedTable_PickStatic(picks);
        state->counter = 0;
        CAI__SetMode(obj, ai, state->lastAnim);
    }
}
