/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200500_de.c, func_802005A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_002A1384(s32 arg0);
s32 func_002A1384(s32 arg0) {
    if ((*(volatile u32 *)(0xA4600010U)) & 3U) {
        do {
        } while ((*(volatile u32 *)(0xA4600010U)) & 3U);
    }
    return (*(volatile u32 *)(arg0));
}
