/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80274020_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
extern void func_00210EF0(f32 *value);
extern float D_800A5644;
extern f32 D_800A564C;
void func_00210EF0(f32 *value)
{
    extern f32 D_800A563C;
    {
        f32 val;
        f32 limit;
        f32 step;
        val = *value;
        limit = *(&D_800A563C + 1);
        if (val < limit) {
            step = D_800A5644;
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
        limit = *(&D_800A5644 + 1);
        if (limit <= val) {
            step = D_800A564C;
            do {
                val -= step;
                *value = val;
            } while (limit <= val);
        }
    }
}
