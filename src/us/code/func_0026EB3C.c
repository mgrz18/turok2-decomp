#include "common.h"
#include "structs/rumble.h"

void CList__Construct(CList *list, s32 lastOffset, s32 nextOffset);
void CList__AddTail(CList *list, void *element);
u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);

/* Puts every one of the count effects on the pool's free list. */
void RumblePool_Init(RumblePool *pool, RumbleEffect *effects, s32 count) {
    u32 mask;
    s32 i;

    mask = __osDisableInt();
    CList__Construct(&pool->free, 0, 4);
    CList__Construct(&pool->active, 0, 4);
    i = 0;
    if (count > 0) {
        do {
            CList__AddTail(&pool->free, effects);
            i++;
            effects++;
        } while (i < count);
    }
    pool->baseLevel = 0;
    __osRestoreInt(mask);
}
