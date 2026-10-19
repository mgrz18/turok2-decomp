/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80275DD4_de_14.c, func_80277128_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800AC8D0[];
extern float D_800A575C;
extern float D_800A5760;
extern float D_800A5758;
extern float D_800A5750;
extern float D_800A5754;
extern float D_800AC9D4[];
extern f32 D_800A5764;
extern void func_0020F040(f32 *);
s32 func_002123D4(ObjectRecAtBC *arg0, u32 arg1, u32 arg2, u32 arg3,
                  Vec3 *arg4, f32 *arg5) {
    f32 temp_f0;
    f32 var_f0;
    f32 var_f1;
    f32 var_f20;
    f32 var_f3;
    u8 temp_v1;
    var_f3 = *(f32 *)&arg1;
    var_f20 = 0.0f;
    if (!(D_800A5750 <= var_f3) && !(var_f3 <= D_800A5754) &&
        !(D_800A5750 <= *(f32 *)&arg2) && !(*(f32 *)&arg2 <= D_800A5754) &&
        !(D_800A5750 <= *(f32 *)&arg3) && !(*(f32 *)&arg3 <= D_800A5754)) {
        temp_v1 = arg0->shape;
        switch (temp_v1) {
        case 0:
            var_f20 = (var_f3 * var_f3) + (*(f32 *)&arg2 * *(f32 *)&arg2) +
                      (*(f32 *)&arg3 * *(f32 *)&arg3);
            if (!(D_800A5750 < var_f20)) {
                var_f20 = D_800AC8D0[(s32)(var_f20 * D_800A5758)];
                goto shared;
            }
            goto fail;
        case 2:
            if (!(var_f3 <= 0.0f)) {
                var_f20 = (*(f32 *)&arg2 * *(f32 *)&arg2) +
                          (*(f32 *)&arg3 * *(f32 *)&arg3);
                if (!((var_f3 * var_f3) < var_f20)) {
                    var_f20 = D_800AC9D4[(s32)(var_f20 * D_800A575C)];
                    var_f20 /= var_f3;
                    goto shared;
                }
            }
            goto fail;
        case 1:
            if (var_f3 < 0.0f) {
                var_f3 = -var_f3;
            }
            var_f0 = *(f32 *)&arg2;
            *(f32 *)&arg1 = var_f3;
            if (var_f0 < 0.0f) {
                var_f0 = -var_f0;
            }
            var_f1 = *(f32 *)&arg3;
            *(f32 *)&arg2 = var_f0;
            if (var_f1 < 0.0f) {
                var_f1 = -var_f1;
            }
            *(f32 *)&arg3 = var_f1;
            var_f20 = var_f1;
            if (!(var_f0 <= var_f20)) {
                var_f20 = var_f0;
            }
            if (!(var_f3 <= var_f20)) {
                var_f20 = var_f3;
            }
            goto shared;
        default:
            goto shared;
        }
shared:
        var_f20 = D_800A5760 - var_f20;
        temp_f0 = arg0->falloff;
        if (var_f20 < temp_f0) {
            var_f20 /= temp_f0;
        } else {
            var_f20 = D_800A5760;
        }
        if (arg0->shape == 1) {
            *arg4 = arg0->axis;
        } else {
            *arg4 = *(Vec3 *)&arg1;
        }
        func_0020F040(arg4);
        *arg5 = var_f20;
        return 1;
    } else {
fail:
        arg4->y = 0.0f;
        arg4->z = 0.0f;
        arg4->x = D_800A5764;
        *arg5 = 0.0f;
        return 0;
    }
}
