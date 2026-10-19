/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200500_de.c, func_80200568_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002A1350(s32 address, s32 value);
void func_002A1350(s32 address, s32 value) {
    if ((*(volatile u32 *)(0xA4600010U)) & 3U) {
        do {
        } while ((*(volatile u32 *)(0xA4600010U)) & 3U);
    }
    *(s32 *)address = value;
}
