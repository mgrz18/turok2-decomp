/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_47.c, osViBlack.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void *D_800B7AC4;
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32);
void func_0029A580(s32 arg0) {
    u32 temp_a0;
    u16 *ptr;
    u16 v;
    temp_a0 = func_0029DFF0();
    if (arg0 & 0xFF) {
        ptr = (u16 *)D_800B7AC4;
        v = *ptr | 0x20;
    } else {
        ptr = (u16 *)D_800B7AC4;
        v = *ptr & 0xFFDF;
    }
    *ptr = v;
    func_0029E010(temp_a0);
}
