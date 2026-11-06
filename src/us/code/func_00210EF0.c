/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80274020_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void Angle_Wrap(f32 *value);
void Angle_Wrap(f32 *value)
{
    extern f32 D_800A563C;
    {
        f32 val;
        f32 limit;
        f32 step;
        val = *value;
        limit = *(&D_800A563C + 1);
        if (val < limit) {
            step = 6.2831855f;
            do {
                val += step;
                *value = val;
            } while (val < limit);
        }
    }
    {
        f32 val;
        f32 limit;
        f32 step;
        val = *value;
        limit = 3.1415927f;
        if (limit <= val) {
            step = 6.2831855f;
            do {
                val -= step;
                *value = val;
            } while (limit <= val);
        }
    }
}
