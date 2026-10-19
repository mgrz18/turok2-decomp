/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7B80_us_28.c, func_802B8C88_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

u8 func_00299E48(u8 *data)
{
    u8 temp = 0;
    u8 temp2;
    int i;
    int j;
    for (i = 0; i <= 32; i++) {
        for (j = 7; j > -1; j--) {
            temp2 = (temp & 0x80) ? 0x85 : 0;
            temp <<= 1;
            if (i == 32) {
                temp &= -1;
            } else {
                temp |= ((*data & (1 << j)) ? 1 : 0);
            }
            temp ^= temp2;
        }
        data++;
    }
    return temp;
}
