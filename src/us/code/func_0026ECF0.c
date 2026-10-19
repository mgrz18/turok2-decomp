/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00285C48.c, func_00285C48.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

u32 func_0029DFF0(void);
void func_0029E010(u32 mask);
u32 func_0029DFF0(void);
void func_0029E010(u32);
s32 func_0026EE40(void *);
void CList__Remove(void *, void *);
void CList__AddTail(void *, void *);
void func_0026ECF0(B19Pool *pool) {
    u32 mask = func_0029DFF0();
    B19Spawn *n;
    B19Spawn *next;
    s32 r;
    for (n = (B19Spawn *)pool->head; n != 0; n = next) {
        next = *(B19Spawn **)((u8 *)n + 4);
        r = 0;
        if (func_0026EE40((u8 *)n + 0x20) || func_0026EE40((u8 *)n + 0x2C)) r = 1;
        if (r) {
            if (n->link != 0) *n->link = 0;
            CList__Remove(&pool->head, n);
            CList__AddTail(pool, n);
        }
    }
    func_0029E010(mask);
}
