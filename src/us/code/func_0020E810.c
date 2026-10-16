/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_80271818_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
extern f32 func_0021153C(f32 arg0);
extern float func_002974C0(float x);
extern float func_002982F0(float x);
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
struct Vector4f;
typedef struct Vector4f Vector4f;
struct Vector4f;
struct Vector4f {
    float x;
    float y;
    float z;
    float w;
};
extern float D_800A554C;
extern float D_800A5554;
extern float D_800A5550;
extern float D_800A5544;
extern float D_800A5548;
extern float D_800A5558;
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 D_800DD78C;
extern f32 func_00298470(f32);
Vector4f func_0020E810(Vec3 *input) {
    Vector4f result;
    Vec3 normalized;
    Vec3 basis;
    Vec3 axis;
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
        result.w = D_800A5544;
    } else {
        f32 normalizationScale;
        normalizationScale = D_800A5548 / magnitude;
        normalized.x = input->x * normalizationScale;
        normalized.y = input->y * normalizationScale;
        normalized.z = input->z * normalizationScale;
        basis.x = 0.0f;
        basis.y = 0.0f;
        basis.z = D_800A5548;
        if (normalized.z < 0.0f) {
            if (D_800A554C < -normalized.z) {
                goto pole;
            }
            goto general;
        }
        if (D_800A5550 < normalized.z) {
pole:
        {
            f32 halfAngle;
            f32 scale;
            halfAngle = 0.0f;
            scale = func_002982F0(halfAngle);
            result.z = normalized.z * scale;
            D_800DD78C = scale;
            result.x = halfAngle;
            result.y = halfAngle;
            result.w = func_002974C0(halfAngle);
        }
        } else {
general:
        {
            f32 halfAngle;
            f32 scale;
            angle = func_0021153C((normalized.x * basis.x) +
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
                axisScale = D_800A5554 / axisMagnitude;
                axis.x *= axisScale;
                axis.y *= axisScale;
                axis.z *= axisScale;
            }
            halfAngle = angle * D_800A5558;
            scale = func_002982F0(halfAngle);
            result.x = axis.x * scale;
            result.y = axis.y * scale;
            result.z = axis.z * scale;
            D_800DD78C = scale;
            result.w = func_002974C0(halfAngle);
        }
        }
    }
    return result;
}
