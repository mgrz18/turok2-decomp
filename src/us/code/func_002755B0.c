/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_8.c, func_8025DEC0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
struct ContextRecAt2BC0;
extern int D_8011ACA0;
struct ContextRecAt2BC0;
extern s32 func_00271630(struct ContextRecAt2BC0 *arg0, s32 arg1, Vec3 arg2,
                         s32 arg3, s32 arg4, f32 arg5);
s32 func_002755B0(s32 arg0, Vec3 arg1, s32 arg4, s32 arg5, f32 arg6) {
    extern s32 D_800F2A00;
    if (D_8011ACA0 != 0) {
        return -1;
    }
    return func_00271630(&D_800F2A00, (s16)arg0, arg1, arg4, arg5, arg6);
}
