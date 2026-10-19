/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_8.c, func_8025E04C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int func_0027573C(int arg0);
extern s32 func_002718C4(struct ContextRecAt2BC0 *arg0);
extern void func_002718BC(struct ContextRecAt2BC0 *arg0, s32 arg1);
extern s32 func_00271128(void *, s32, Vec3, s32);
extern int func_002718C4(struct ContextRecAt2BC0 *arg0);
extern void func_002718BC(struct ContextRecAt2BC0 *arg0, int arg1);
extern int func_00271128(void *arg0, int arg1, Vec3 arg2, int arg3);
extern int D_8011ACA0;
int func_0027573C(int arg0) {
    extern signed char D_800F2A00;
    int temp_s0;
    int temp_s1;
    if (D_8011ACA0 != 0) {
        return -1;
    }
    {
        Vec3 vec;
        register float zero = (float)0;
        vec.z = zero;
        vec.y = zero;
        vec.x = zero;
        temp_s1 = func_002718C4(&D_800F2A00);
        func_002718BC(&D_800F2A00, 1);
        temp_s0 = func_00271128(&D_800F2A00, arg0, vec, 0);
        func_002718BC(&D_800F2A00, temp_s1);
    }
    return temp_s0;
}
