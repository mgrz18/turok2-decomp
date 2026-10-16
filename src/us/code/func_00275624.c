/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_8.c, func_8025DF34_de.
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
extern s32 func_00270C80(void *, s32, Vec3, s32, s32);
struct ContextRecAt2BC0;
extern s32 func_002718C4(struct ContextRecAt2BC0 *arg0);
extern void func_002718BC(struct ContextRecAt2BC0 *arg0, s32 arg1);
extern int func_002718C4(struct ContextRecAt2BC0 *arg0);
extern void func_002718BC(struct ContextRecAt2BC0 *arg0, int arg1);
extern int D_8011ACA0;
struct ContextRecAt2BC0;
s32 func_00275624(s32 arg0) {
    extern s32 D_800F2A00;
    s32 temp_s0;
    s32 temp_s1;
    if (D_8011ACA0 != 0) {
        return -1;
    }
    {
        Vec3 vec;
        register f32 zero = 0.0f;
        vec.z = zero;
        vec.y = zero;
        vec.x = zero;
        temp_s1 = func_002718C4(&D_800F2A00);
        func_002718BC(&D_800F2A00, 1);
        temp_s0 = func_00270C80(&D_800F2A00, (s16)arg0, vec, 0, -1);
        func_002718BC(&D_800F2A00, temp_s1);
    }
    return temp_s0;
}
