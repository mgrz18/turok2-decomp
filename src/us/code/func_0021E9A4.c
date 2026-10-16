/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80246E44_de_14.c, func_8024B6A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern s32 func_0021E9F8(void *arg0, s32 arg1, s32 arg2);
extern s32 func_00219F74(void *, s32, s32);
s32 func_0021E9A4(void *a, s32 c, s32 flag) {
    s32 temp_v0 = func_00219F74(a, c, -1);
    if (temp_v0 != -1) {
        return func_0021E9F8(a, temp_v0, flag);
    }
    return 0;
}
s32 func_0021E9F8(void *arg0, s32 arg1, s32 arg2);
extern s32 func_00219F74(void *arg0, s32 arg1, s32 arg2);
