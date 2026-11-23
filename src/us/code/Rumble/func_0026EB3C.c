#include "common.h"
#include "structs/rumble.h"
#include "protos/list.h"
#ifdef TARGET_PORT
#include "structs/controller.h"
_Static_assert(sizeof(((ControllerProfile *) 0)->rumblePool) == __builtin_offsetof(RumblePool, baseLevel), "rumblePool");
_Static_assert(__builtin_offsetof(ControllerProfile, rumbleBase) - __builtin_offsetof(ControllerProfile, rumblePool) == __builtin_offsetof(RumblePool, baseLevel), "rumbleBase");
_Static_assert(sizeof(((ControllerProfile *) 0)->rumbleNodes[0]) == sizeof(RumbleEffect), "rumbleNodes");
_Static_assert(__builtin_offsetof(ControllerProfile, rumbleNodes) - __builtin_offsetof(ControllerProfile, rumblePool) >= sizeof(RumblePool), "rumble pool size");
#endif

u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);

/* Puts every one of the count effects on the pool's free list. */
void RumblePool_Init(RumblePool *pool, RumbleEffect *effects, s32 count) {
    u32 mask;
    s32 i;

    mask = __osDisableInt();
    CList__Construct(&pool->free, 0, sizeof(void *));
    CList__Construct(&pool->active, 0, sizeof(void *));
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
