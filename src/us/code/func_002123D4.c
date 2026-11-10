/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80275DD4_de_14.c, func_80277128_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/scene.h"

extern void CVector3__Normalize(f32 *);
#ifdef TARGET_PORT
/* ABI: the callers pass the point as three floats (SSE registers on x86-64, s registers on arm64),
 * the ROM's code reads them as words from its argument slots (a1..a3, contiguous in the home
 * area: *(CVector3 *)&arg1). Here they are floats, kept contiguous in a local. */
s32 Actors_GetLightVolumeFalloff(ObjectRecAtBC *arg0, f32 px, f32 py, f32 pz,
                  CVector3 *arg4, f32 *arg5) {
    union { f32 f[3]; u32 u[3]; } args_ = {{px, py, pz}};
#define arg1 args_.u[0]
#define arg2 args_.u[1]
#define arg3 args_.u[2]
#else
s32 Actors_GetLightVolumeFalloff(ObjectRecAtBC *arg0, u32 arg1, u32 arg2, u32 arg3,
                  CVector3 *arg4, f32 *arg5) {
#endif
    f32 temp_f0;
    f32 var_f0;
    f32 var_f1;
    f32 var_f20;
    f32 var_f3;
    u8 temp_v1;
    var_f3 = *(f32 *)&arg1;
    var_f20 = 0.0f;
    if (!(1.0f <= var_f3) && !(var_f3 <= -1.0f) &&
        !(1.0f <= *(f32 *)&arg2) && !(*(f32 *)&arg2 <= -1.0f) &&
        !(1.0f <= *(f32 *)&arg3) && !(*(f32 *)&arg3 <= -1.0f)) {
        temp_v1 = arg0->shape;
        switch (temp_v1) {
        case 0:
            var_f20 = (var_f3 * var_f3) + (*(f32 *)&arg2 * *(f32 *)&arg2) +
                      (*(f32 *)&arg3 * *(f32 *)&arg3);
            if (!(1.0f < var_f20)) {
                var_f20 = sLightSphereFalloff[(s32)(var_f20 * 64.0f)];
                goto shared;
            }
            goto fail;
        case 2:
            if (!(var_f3 <= 0.0f)) {
                var_f20 = (*(f32 *)&arg2 * *(f32 *)&arg2) +
                          (*(f32 *)&arg3 * *(f32 *)&arg3);
                if (!((var_f3 * var_f3) < var_f20)) {
                    var_f20 = sLightConeFalloff[(s32)(var_f20 * 64.0f)];
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
        var_f20 = 1.0f - var_f20;
        temp_f0 = arg0->falloff;
        if (var_f20 < temp_f0) {
            var_f20 /= temp_f0;
        } else {
            var_f20 = 1.0f;
        }
        if (arg0->shape == 1) {
            *arg4 = arg0->axis;
        } else {
            *arg4 = *(CVector3 *)&arg1;
        }
        CVector3__Normalize(arg4);
        *arg5 = var_f20;
        return 1;
    } else {
fail:
        arg4->y = 0.0f;
        arg4->z = 0.0f;
        arg4->x = 1.0f;
        *arg5 = 0.0f;
        return 0;
    }
}
#ifdef TARGET_PORT
#undef arg1
#undef arg2
#undef arg3
#endif
