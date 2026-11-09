/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAC50_de_24.c, func_802BAC90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern void __osCleanupThread(void);
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);
void osCreateThread(OSThread *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 p)
{
    register u32 saveMask;
    u32 mask;
    OSThread **active;
    t->id = id;
    t->priority = p;
    t->next = 0;
    t->queue = 0;
    t->context.pc = (u32)entry;
    t->context.a0 = (s64)(s32)arg;
    t->context.sp = (s64)(s32)sp - 16;
    t->context.ra = (s64)(s32)__osCleanupThread;
    mask = 0x3FFF01;
    t->context.sr = (mask & 0xFF01) | 0x04000002;
    t->context.rcp = (mask & 0x3F0000) >> 16;
    t->context.fpcsr = 0x01000800;
    t->fp = 0;
    t->state = 1;
    t->flags = 0;
    saveMask = __osDisableInt();
    active = &__osActiveQueue;
    t->tlnext = *active;
    *active = t;
    __osRestoreInt(saveMask);
}
