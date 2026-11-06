/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_47.c, osViBlack.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern __OSViContext *D_800B7AC4; /* __osViNext */
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
void osViBlack(s32 arg0) {
    u32 temp_a0;
    u16 *ptr;
    u16 v;
    temp_a0 = __osDisableInt();
    if (arg0 & 0xFF) {
        ptr = &D_800B7AC4->state;
        v = *ptr | 0x20;
    } else {
        ptr = &D_800B7AC4->state;
        v = *ptr & 0xFFDF;
    }
    *ptr = v;
    __osRestoreInt(temp_a0);
}
