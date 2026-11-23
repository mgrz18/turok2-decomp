#include "common.h"
#include "structs/game_object.h"

/* Returns the distance to the AI's second target when it is an actor with flag 0x300000 (and stores that
 * actor in *found), else -1 (and NULL). */

extern void AI_UpdateTargets(CGameObjectInstance *obj, void *ai, CAITargets *t, s32 f1, s32 f2);

f32 MotherBoss_GetTargetDist(CGameObjectInstance *obj, CGameObjectInstance **found) {
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
