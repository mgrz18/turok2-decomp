/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_80270CD0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
extern f32 func_0021153C(f32 arg0);
extern float func_002982F0(float x);
struct Vector4f;
typedef struct Vector4f Vector4f;
struct Vector4f;
struct Vector4f {
    float x;
    float y;
    float z;
    float w;
};
extern float D_800A5504;
extern float D_800A550C;
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
void func_0020DCC8(Vector4f *out, f32 amount, Vector4f *a, Vector4f *b) {
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
    if (D_800A5504 < dot) {
        f32 magnitude;
        f32 scale;
        out->x = a->x + amount * (other->x - a->x);
        out->y = a->y + amount * (other->y - a->y);
        out->z = a->z + amount * (other->z - a->z);
        out->w = a->w + amount * (other->w - a->w);
        magnitude = func_00298470((out->x * out->x) + (out->y * out->y) +
                                  (out->z * out->z) + (out->w * out->w));
        if (magnitude != 0.0f) {
            scale = *(&D_800A5504 + 1) / magnitude;
            out->x *= scale;
            out->y *= scale;
            out->z *= scale;
            out->w *= scale;
        }
    } else {
        angle = func_0021153C(dot);
        sine = func_002982F0(angle);
        if (sine == 0.0f) {
            *out = *a;
            return;
        }
        inverseSine = D_800A550C / sine;
        scaleA = func_002982F0((D_800A550C - amount) * angle) * inverseSine;
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
