/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_14.c, func_802BFF90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002A1050(float *arg0, float *arg1, float *arg2);
void func_002A1050(float *arg0, float *arg1, float *arg2) {
    short i = 0;
    do {
        float re = arg0[i * 2];
        float im = arg0[i * 2 + 1];
        float a = arg1[i];
        float b = arg2[i];
        arg0[i * 2] = re * a - im * b;
        arg0[i * 2 + 1] = re * b + im * a;
        i++;
    } while (i < 0x80);
}
