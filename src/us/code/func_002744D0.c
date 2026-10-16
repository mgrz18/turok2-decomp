/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025C25C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef float f32;
extern f32 func_002119FC(f32 arg0, f32 arg1);
extern s16 func_002744D0(s16 arg0, s16 arg1);
s16 func_002744D0(s16 arg0, s16 arg1) {
    f32 temp_f20;
    f32 temp_f21;
    temp_f21 = (f32)arg1;
    temp_f20 = (f32)arg0;
    return (s16)(s32)(func_002119FC(temp_f21, temp_f20) < 0.0f
                          ? temp_f20 - func_002119FC(temp_f21, temp_f20)
                          : temp_f20 + func_002119FC(temp_f21, temp_f20));
}
