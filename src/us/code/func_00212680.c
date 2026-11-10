/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802773D4_de.c, func_802773D4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void CMtxF__TransformPoint(void *, void *, void *);
extern void CMtxF__RotateVector(void *arg0, void *arg1, void *arg2);
extern void CVector3__Normalize(f32 *);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Actors_GetLightVolumeFalloff Actors_GetLightVolumeFalloff_f3
#endif
extern s32 Actors_GetLightVolumeFalloff(ObjectRecAtBC *arg0, f32 arg1, f32 arg2, f32 arg3,
                        f32 *arg4, f32 *arg5);
extern void CVector3__Scale(void *, void *, f32);
s32 Actors_EvaluateLightVolume(ObjectRecAtBC *arg0, CVector3 arg1, CVector3 arg4, Matrix *arg7,
                  CVector3 *arg8, f32 *arg9) {
    CVector3 second;
    CVector3 first;
    CVector3 normal;
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
        CMtxF__TransformPoint(arg7, &arg1.x, &first);
        CMtxF__RotateVector(arg7, &arg4.x, &second);
    }
    CVector3__Normalize(&second.x);
    if (Actors_GetLightVolumeFalloff(arg0, first.x, first.y, first.z,
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
                          (dot * (1.0f - objectValue)));
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
        CVector3__Scale(arg8, &arg0->direction, amount);
    }
    return 1;
}
