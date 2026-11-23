/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_13.c, func_802BF67C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/adec.h"

extern float func_002974C0(float x);
extern float func_002982F0(float x);
extern FftTables *mdctFftInit(s32 n);
FftTables *mdctFftInit(s32 n) {
    FftTables *tables;
    s32 half;
    s32 i;
    s32 k;
    s32 j;
    s32 m;
    s32 swap;
    s32 limit;
    s32 *order;
    f32 angle;
    f32 scale;
    f32 count;
    half = n >> 1;
    tables = &gMdctFftTables;
    i = 0;
    if (i < half) {
        scale = 6.2831855f;
        count = n;
        do {
            angle = i * scale / count;
            tables->cos[i] = func_002974C0(angle);
            tables->sin[i] = -func_002982F0(angle);
            i++;
        } while (i < half);
    }
    order = tables->bitrev;
    limit = n * 2;
    for (k = 0; k < n; k++) {
        order[k] = k;
    }
    j = 1;
    for (k = 1; k <= limit; k += 2) {
        if (k < j) {
            swap = order[(j - 1) / 2];
            order[(j - 1) / 2] = order[(k - 1) / 2];
            order[(k - 1) / 2] = swap;
        }
        m = n;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    tables->n = n;
    return tables;
}
