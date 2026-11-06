/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_80270CD0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 Math_AcosLookup(f32 arg0);
extern float func_002982F0(float x);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
void CQuaternion__SlerpFast(Vector4f *out, f32 amount, Vector4f *a, Vector4f *b) {
    Vector4f negative;
    Vector4f *other;
    f32 dot;
    f32 negativeDot;
    f32 angle;
    f32 sine;
    f32 inverseSine;
    f32 scaleA;
    f32 scaleB;
    other = b;
    negative = *b;
    negative.x = -negative.x;
    negative.y = -negative.y;
    negative.z = -negative.z;
    negative.w = -negative.w;
    dot = (a->x * b->x) + (a->y * b->y) + (a->z * b->z) + (a->w * b->w);
    negativeDot = (a->x * negative.x) + (a->y * negative.y) +
                  (a->z * negative.z) + (a->w * negative.w);
    if (dot < negativeDot) {
        dot = negativeDot;
        other = &negative;
    }
    if (0.70710677f < dot) {
        f32 magnitude;
        f32 scale;
        out->x = a->x + amount * (other->x - a->x);
        out->y = a->y + amount * (other->y - a->y);
        out->z = a->z + amount * (other->z - a->z);
        out->w = a->w + amount * (other->w - a->w);
        magnitude = func_00298470((out->x * out->x) + (out->y * out->y) +
                                  (out->z * out->z) + (out->w * out->w));
        if (magnitude != 0.0f) {
            scale = 1.0f / magnitude;
            out->x *= scale;
            out->y *= scale;
            out->z *= scale;
            out->w *= scale;
        }
    } else {
        angle = Math_AcosLookup(dot);
        sine = func_002982F0(angle);
        if (sine == 0.0f) {
            *out = *a;
            return;
        }
        inverseSine = 1.0f / sine;
        scaleA = func_002982F0((1.0f - amount) * angle) * inverseSine;
        scaleB = func_002982F0(amount * angle) * inverseSine;
        if (dot < negativeDot) {
            other = &negative;
        }
        out->x = (scaleA * a->x) + (scaleB * other->x);
        out->y = (scaleA * a->y) + (scaleB * other->y);
        out->z = (scaleA * a->z) + (scaleB * other->z);
        out->w = (scaleA * a->w) + (scaleB * other->w);
    }
}
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
