/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802846C8_de_17.c, func_80285D30_de.
 * The same code in Turok 2. */

#include "common.h"
#include "structs/rumble.h"
#include "protos/list.h"

u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);

/* Returns every active effect to the free list. */
void RumblePool_FreeAll(RumblePool *pool) {
    RumbleEffect *effect;
    RumbleEffect *next;
    u32 mask;

    mask = __osDisableInt();
    effect = pool->active.m_pHead;
    if (effect != NULL) {
        do {
            next = effect->next;
            CList__Remove(&pool->active, effect);
            CList__AddTail(&pool->free, effect);
            effect = next;
        } while (effect != NULL);
    }
    pool->baseLevel = 0;
    __osRestoreInt(mask);
}
