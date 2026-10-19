/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802773D4_de.c, func_802773D4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800A5768;
extern void func_0020F85C(void *, void *, void *);
extern void func_0020FAFC(void *arg0, void *arg1, void *arg2);
extern void func_0020F040(f32 *);
extern s32 func_002123D4(ObjectRecAtBC *arg0, f32 arg1, f32 arg2, f32 arg3,
                        f32 *arg4, f32 *arg5);
extern void func_0020EF60(void *, void *, f32);
s32 func_00212680(ObjectRecAtBC *arg0, Vec3 arg1, Vec3 arg4, Matrix *arg7,
                  Vec3 *arg8, f32 *arg9) {
    Vec3 second;
    Vec3 first;
    Vec3 normal;
    f32 amount;
    f32 dot;
    f32 value;
    f32 objectValue;
    if (arg0->flagB9 != 0) {
        first.x = arg1.x * arg7->m[0];
        first.y = arg1.y * arg7->m[5];
        first.z = arg1.z * arg7->m[10];
        first.x += arg7->m[12];
        first.y += arg7->m[13];
        first.z += arg7->m[14];
        second.x = arg4.x * arg7->m[0];
        second.y = arg4.y * arg7->m[5];
        second.z = arg4.z * arg7->m[10];
    } else {
        func_0020F85C(arg7, &arg1.x, &first);
        func_0020FAFC(arg7, &arg4.x, &second);
    }
    func_0020F040(&second.x);
    if (func_002123D4(arg0, first.x, first.y, first.z,
                     &normal.x, &amount) == 0) {
        return 0;
    }
    {
        dot = -((normal.x * second.x) + (normal.y * second.y) +
                (normal.z * second.z));
        if (dot < 0.0f) {
            dot = 0.0f;
        }
        objectValue = arg0->value;
        value = amount * (objectValue +
                          (dot * (D_800A5768 - objectValue)));
        amount = value;
        if (value <= 0.0f) {
            amount = 0.0f;
        }
        if (arg0->flagB8 != 0) {
            amount = -amount;
        }
        if (arg9 != 0) {
            *arg9 = amount;
        }
        func_0020EF60(arg8, &arg0->direction, amount);
    }
    return 1;
}
