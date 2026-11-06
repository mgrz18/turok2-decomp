/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00285C48.c, func_00285C48.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
u32 __osDisableInt(void);
void __osRestoreInt(u32);
s32 RumbleCurve_Advance(void *);
void CList__Remove(void *, void *);
void CList__AddTail(void *, void *);
void RumblePool_FreeFinished(B19Pool *pool) {
    u32 mask = __osDisableInt();
    B19Spawn *n;
    B19Spawn *next;
    s32 r;
    for (n = (B19Spawn *)pool->head; n != 0; n = next) {
        next = *(B19Spawn **)((u8 *)n + 4);
        r = 0;
        if (RumbleCurve_Advance((u8 *)n + 0x20) || RumbleCurve_Advance((u8 *)n + 0x2C)) r = 1;
        if (r) {
            if (n->link != 0) *n->link = 0;
            CList__Remove(&pool->head, n);
            CList__AddTail(pool, n);
        }
    }
    __osRestoreInt(mask);
}
