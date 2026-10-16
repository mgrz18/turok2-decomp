/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80273EE4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
extern void func_00210EF0(f32 *value);
extern float D_800A5634;
extern const f32 D_800A562C;
extern f32 D_800A562C;
f32 func_00210DB4(f32 fraction, f32 first, f32 second) {
    f32 var_f3;
    f32 result;
    func_00210EF0(&first);
    func_00210EF0(&second);
    if (first > second) {
        var_f3 = second + *(&D_800A562C + 1);
        if ((var_f3 - first) < (first - second)) {
            second = var_f3;
        }
    } else {
        var_f3 = second - D_800A5634;
        if ((first - var_f3) < (second - first)) {
            second = var_f3;
        }
    }
    result = first + (fraction * (second - first));
    func_00210EF0(&result);
    return result;
}
void func_00210EF0(f32 *value);
