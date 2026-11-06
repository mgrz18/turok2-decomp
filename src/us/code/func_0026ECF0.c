/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00285C48.c, func_00285C48.
 * The same code in Turok 2. */

#include "common.h"
#include "structs/rumble.h"

u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
s32 RumbleCurve_Advance(RumbleCurve *curve);
void CList__Remove(CList *list, void *element);
void CList__AddTail(CList *list, void *element);

/* Returns the effects whose amplitude or lifetime curve has run out to the
 * free list (Rumble_IsFinished, inlined). */
void RumblePool_FreeFinished(RumblePool *pool) {
    u32 mask = __osDisableInt();
    RumbleEffect *effect;
    RumbleEffect *next;
    s32 finished;

    for (effect = pool->active.m_pHead; effect != NULL; effect = next) {
        next = effect->next;
        finished = 0;
        if (RumbleCurve_Advance(&effect->amplitude) || RumbleCurve_Advance(&effect->lifetime)) finished = 1;
        if (finished) {
            if (effect->owner != NULL) *effect->owner = NULL;
            CList__Remove(&pool->active, effect);
            CList__AddTail(&pool->free, effect);
        }
    }
    __osRestoreInt(mask);
}
