/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B6EBC.c, func_802B1DEC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00292DB0(void *arg0, f32 arg1);
void func_00292D4C(void *arg0, void *arg1) {
    u8 *p = (u8 *)arg1;
    int pad[4];
    (void)pad;
    if (p[8] == 0xFF && p[9] == 0x51) {
        func_00292DB0(arg0, (f32)((p[0xB] << 0x10) | (p[0xC] << 8) | p[0xD]));
    }
}
