/* FAKEMATCH: identical if/else arms in the counting loop (found by the permuter); the plain loop puts the
 * counter in $v0 instead of $a1. */
#include "common.h"
#include "structs/game_object.h"

/* Returns whether fewer than max actors are in the gScene_m_InstancePool_m_ActiveList list. */

extern CListedInstance *gScene_m_InstancePool_m_ActiveList;

s32 MotherBoss_FewerActorsThan(s32 max) {
    CListedInstance *actor;
    s32 count;

    count = 0;
    for (actor = gScene_m_InstancePool_m_ActiveList; actor != NULL; actor = actor->m_pNext) {
        /* both arms are merged by cross-jumping; the extra block gives the ROM's register allocation */
        if (actor->m_pNext != NULL) {
            count++;
        } else {
            count++;
        }
    }
    return count < max;
}
