/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_18.c, func_802BBA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern u32 D_80122A30;
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);
extern u32 osGetCount(void);
extern void __osSetCompare(u32 compare);
extern void __osSetTimerIntr(u64 tim);
void __osSetTimerIntr(u64 interval) {
    u64 compare;
    u32 saved = __osDisableInt();
    D_80122A30 = osGetCount();
    compare = D_80122A30 + interval;
    __osSetCompare(compare);
    __osRestoreInt(saved);
}
