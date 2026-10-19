/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802773D4_de_15.c, func_80278F00_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void func_0026E294(Record14 *arg0) {
    extern void func_0026DFB0(void *);
    Record14 copy;
    u8 *bytes = (u8 *)arg0;
    bytes[0xE] |= 0x20;
    if (bytes[0x11] == 1) {
        copy = *arg0;
        ((u8 *)&copy)[0x12] = 2;
        ((u8 *)&copy)[0x10] = 0;
        func_0026DFB0(&copy);
    }
}
