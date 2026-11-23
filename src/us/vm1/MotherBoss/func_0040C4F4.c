/* FAKEMATCH: identical if/else arms in the counting loop (found by the permuter); the plain loop puts the
 * counter in $v0 instead of $a1. Why it works: the arms' test loads m_pNext into a temporary that lives in
 * one block, local-alloc gives it $v0, so every value live across the loop conflicts with $v0 and the
 * counter falls to $a1; cross-jumping merges the arms only after reload, and the dead load goes with them.
 * Natural forms tried in the L pass (all give the counter $v0): for/while/do-while/comma-for loops, a `next`
 * pointer, a break on the last actor, an inline count function, `count < max ? 1 : 0`, if/return pairs,
 * `register` locals, `max` copied to a local, a ternary next, and a dead second counter or `last` pointer
 * (deleted before allocation, so they leave no temporary). Only arms that survive to reload do it. */
#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

/* Returns whether fewer than max actors are in the gScene_m_InstancePool_m_ActiveList list. */


s32 MotherBoss_FewerActorsThan(s32 max) {
    CListedInstance *actor;
    s32 count;

    count = 0;
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
