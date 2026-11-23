/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/UTIL/802C09A0.CPP, func_802BB8B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

void __osSetTimerIntr(u64 interval);
u64 __osInsertTimer(OSTimer *t);
extern void __osSetCompare(u32 compare);

void __osTimerInterrupt(void)
{
    OSTimer *t;
    u32 count;
    u32 elapsed_cycles;
    u32 *counter;
    if (((OSTimer *)__osTimerList)->next == __osTimerList) {
        return;
    }
    counter = &__osTimerCounter;
    for (;;) {
        t = ((OSTimer *)__osTimerList)->next;
        if (t == __osTimerList) {
            __osSetCompare(0);
            __osTimerCounter = 0;
            break;
        }
        count = osGetCount();
        elapsed_cycles = count - *counter;
        *counter = count;
        if (elapsed_cycles < t->value) {
            t->value -= elapsed_cycles;
            __osSetTimerIntr(t->value);
            break;
        }
        t->prev->next = t->next;
        t->next->prev = t->prev;
        t->next = 0;
        t->prev = 0;
        if (t->mq != 0) {
            osSendMesg(t->mq, t->msg, 0);
        }
        if (t->interval != 0) {
            t->value = t->interval;
            __osInsertTimer(t);
        }
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_18.c, func_802BB9F8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void __osTimerServicesInit(void);

void __osTimerServicesInit(void) {
    NodeRecAt1C *node = (NodeRecAt1C *)__osTimerList;
    __osCurrentTime = 0;
    __osBaseCounter = 0;
    __osViIntrCount = 0;
    node->field10 = 0;
    node->field8 = 0;
    node->prev = node;
    node->next = node;
    node->field18 = 0;
    node->field1C = 0;
}

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);

void __osSetTimerIntr(u64 interval) {
    u64 compare;
    u32 saved = __osDisableInt();
    __osTimerCounter = osGetCount();
    compare = __osTimerCounter + interval;
    __osSetCompare(compare);
    __osRestoreInt(saved);
}

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
