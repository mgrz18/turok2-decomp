/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_14.c, func_802BFF30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002A0FF0(float *arg0, float *arg1);
void func_002A0FF0(float *arg0, float *arg1) {
    short i = 0;
    do {
        arg0[i * 2] = arg1[i * 2];
        arg0[i * 2 + 1] = arg1[(255 - i * 2)];
        i++;
    } while (i < 128);
}
