/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_80271818_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/math.h"
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck struct_scalar): the callers pass a result pointer first, as
 * MIPS does for a struct returned by value; x86-64 and arm64 return this struct in registers.
 * The definition keeps its by-value form as CQuaternion__FromDirection_byval; CQuaternion__FromDirection below takes the pointer. */
#define CQuaternion__FromDirection CQuaternion__FromDirection_byval
#endif

extern f32 Math_AcosLookup(f32 arg0);
extern float func_002974C0(float x);
extern float func_002982F0(float x);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
Vector4f CQuaternion__FromDirection(CVector3 *input) {
    Vector4f result;
    CVector3 normalized;
    CVector3 basis;
    CVector3 axis;
    f32 magnitude;
    f32 angle;
    f32 axisMagnitude;
    magnitude = func_00298470((input->x * input->x) +
                              (input->y * input->y) +
                              (input->z * input->z));
    if (magnitude == 0.0f) {
        result.z = 0.0f;
        result.y = 0.0f;
        result.x = 0.0f;
        result.w = 1.0f;
    } else {
        f32 normalizationScale;
        normalizationScale = 1.0f / magnitude;
        normalized.x = input->x * normalizationScale;
        normalized.y = input->y * normalizationScale;
        normalized.z = input->z * normalizationScale;
        basis.x = 0.0f;
        basis.y = 0.0f;
        basis.z = 1.0f;
        if (normalized.z < 0.0f) {
            if (0.99999f < -normalized.z) {
                goto pole;
            }
            goto general;
        }
        if (0.99999f < normalized.z) {
pole:
        {
            f32 halfAngle;
            f32 scale;
            halfAngle = 0.0f;
            scale = func_002982F0(halfAngle);
            result.z = normalized.z * scale;
            gQuaternionSin = scale;
            result.x = halfAngle;
            result.y = halfAngle;
            result.w = func_002974C0(halfAngle);
        }
        } else {
general:
        {
            f32 halfAngle;
            f32 scale;
            angle = Math_AcosLookup((normalized.x * basis.x) +
                                  (normalized.y * basis.y) +
                                  (normalized.z * basis.z));
            axis.x = (basis.y * normalized.z) - (basis.z * normalized.y);
            axis.y = (basis.z * normalized.x) - (basis.x * normalized.z);
            axis.z = (basis.x * normalized.y) - (basis.y * normalized.x);
            axisMagnitude = func_00298470((axis.x * axis.x) +
                                          (axis.y * axis.y) +
                                          (axis.z * axis.z));
            if (axisMagnitude != 0.0f) {
                f32 axisScale;
                axisScale = 1.0f / axisMagnitude;
                axis.x *= axisScale;
                axis.y *= axisScale;
                axis.z *= axisScale;
            }
            halfAngle = angle * 0.5f;
            scale = func_002982F0(halfAngle);
            result.x = axis.x * scale;
            result.y = axis.y * scale;
            result.z = axis.z * scale;
            gQuaternionSin = scale;
            result.w = func_002974C0(halfAngle);
        }
        }
    }
    return result;
}

#ifdef TARGET_PORT
#undef CQuaternion__FromDirection
void CQuaternion__FromDirection(Vector4f *out, CVector3 *input) {
    *out = CQuaternion__FromDirection_byval(input);
}
#endif
