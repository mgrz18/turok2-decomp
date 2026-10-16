/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_8.c, func_8025DFE8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

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
extern s32 func_00271128(void *, s32, Vec3, s32);
extern int func_00271128(void *arg0, int arg1, Vec3 arg2, int arg3);
extern int D_8011ACA0;
s32 func_002756D8(s32 arg0, Vec3 arg1, s32 arg2) {
    extern s32 D_800F2A00;
    if (D_8011ACA0 != 0) {
        return -1;
    }
    return func_00271128(&D_800F2A00, arg0, arg1, arg2);
}
