/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAC50_de_30.c, func_802BADC0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern u32 __osDisableInt(void);
extern u32 osGetCount(void);
extern void __osRestoreInt(u32);
extern s32 D_80122A20;
extern u64 D_80122A28;
u64 osGetTime(void) {
    u32 saved;
    u32 count;
    u32 elapsed;
    u64 time;
    saved = __osDisableInt();
    count = osGetCount();
    elapsed = count - D_80122A20;
    time = D_80122A28;
    __osRestoreInt(saved);
    return time + elapsed;
}
