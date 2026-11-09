/* MATCHES at its real extent (checked with the symbol size patched to 0x20C); both flag masks are
 * computed before the tests (found with the permuter). */
/* WRONG CUT: real function 0x0040A6F0-0x0040A8FC (size 0x20C); splat split it at its tail jumps into
 * MotherBoss_Phase1ChooseAttack, func_0040A7AC, func_0040A8A0 and func_0040A8A4. */
#include "common.h"
#include "structs/pick.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "protos/ai.h"

/* Picks the next attack anim from a weighted table: close to the target only 0x10/0xD, otherwise 0xF
 * (when flagged and not just used), 0x10 and 0x12 weighted by the module's timer ratio. */

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern s16 WeightedTable_PickStatic(Pick *table);

/* MotherBoss_GetTargetDist */
static __inline__ f32 getTargetDist(CGameObjectInstance *obj, CGameObjectInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    AI_UpdateTargets(obj, &obj->m_AI, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = t.m_Path.m_pTarget;
    }
    return dist;
}

void MotherBoss_Phase1ChooseAttack(CGameObjectInstance *obj, CAI *ai) {
    Pick picks[8];
    CGameObjectInstance *found;
    MotherBossState *state = &gMotherBoss;
    s32 n;
    s32 flagged;
    f32 dist;
    f32 ratio;
    s32 flag2;
    s32 flag20;

    n = 0;
    dist = getTargetDist(obj, &found);
    flagged = 0;
    flag2 = ai->m_VisBits & 2;
    flag20 = ai->m_VisBits & 0x20;
    if (flag2) {
        if (flag20) {
            flagged = 1;
        }
    }
    if (state->totalHealth != 0) {
        ratio = (f32)state->health / (f32)state->totalHealth;
    } else {
        ratio = 1.0f;
    }
    if (found != NULL) {
        if (dist < 1433.6f) {
            picks[n].id = 0x10;
            picks[n++].weight = 5;
            picks[n].id = 0xD;
            picks[n++].weight = 5;
        } else {
            if (flagged && state->lastAnim != 0xF) {
                picks[n].id = 0xF;
                picks[n++].weight = 5;
            }
            picks[n].id = 0x10;
            picks[n++].weight = 5;
            picks[n].id = 0x12;
            if (ratio < 0.75f) {
                if (ratio < 0.5f) {
                    picks[n].weight = 10;
                } else {
                    picks[n].weight = 5;
                }
            } else {
                picks[n].weight = 2;
            }
            n++;
        }
        picks[n].id = -1;
        state->lastAnim = WeightedTable_PickStatic(picks);
        state->counter = 0;
        CAI__SetMode(obj, ai, state->lastAnim);
    }
}
