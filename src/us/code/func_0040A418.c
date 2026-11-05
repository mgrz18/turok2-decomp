/* FAKEMATCH: identical if/else arms in fewerActorsThan's counting loop (as in func_0040C4F4). */
#include "common.h"
#include "structs/pick.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Picks the next attack anim from a weighted table: close to the target 4 (then 3 after a 4), otherwise 5
 * when fewer than 3 actors are in the D_8010FEB4 list, else 6. */

extern CListedInstance *D_8010FEB4;
extern void func_00246108(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern s16 func_0026D518(Pick *table);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 state);

/* func_0040BD60 */
static __inline__ f32 getTargetDist(CGameObjectInstance *obj, CListedInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    func_00246108(obj, &obj->m_AI, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = (CListedInstance *) t.m_Path.m_pTarget;
    }
    return dist;
}

/* func_0040C4F4 */
static __inline__ s32 fewerActorsThan(s32 max) {
    CListedInstance *actor;
    s32 count = 0;

    for (actor = D_8010FEB4; actor != NULL; actor = actor->m_pNext) {
        /* both arms are merged by cross-jumping; the extra block gives the ROM's register allocation */
        if (actor->m_pNext != NULL) {
            count++;
        } else {
            count++;
        }
    }
    return count < max;
}

void func_0040A418(CGameObjectInstance *obj, CAI *ai) {
    Pick picks[8];
    CListedInstance *found;
    MotherBossState *state = &D_8012FB70;
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
        state->lastAnim = func_0026D518(picks);
        state->counter = 0;
        func_00243414(obj, ai, state->lastAnim);
    }
}
