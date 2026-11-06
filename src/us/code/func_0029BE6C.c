/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_19.c, __osInsertTimer.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);
u64 __osInsertTimer(OSTimer *t)
{
    OSTimer *timep;
    u64 tim;
    u32 savedMask = __osDisableInt();
    timep = __osTimerList->next;
    tim = t->value;
    for (; timep != __osTimerList && tim > timep->value; timep = timep->next) {
        tim -= timep->value;
    }
    t->value = tim;
    if (timep != __osTimerList) {
        timep->value -= tim;
    }
    t->next = timep;
    t->prev = timep->prev;
    timep->prev->next = t;
    timep->prev = t;
    __osRestoreInt(savedMask);
    return tim;
}
