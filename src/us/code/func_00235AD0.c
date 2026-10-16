/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A25D0_de_17.c, func_802A5020_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned int u32;
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
struct func_802077F4_S2;
typedef struct func_802077F4_S2 func_802077F4_S2;
struct func_802077F4_S2;
struct func_802077F4_S2 {
    char pad0[0x4];
    f32 unk4;
};
extern float D_800A6508;
extern Vector4f *func_00235AD0(Vector4f *out, u32 bx, u32 by, u32 bz);
extern f32 D_800DD78C;
extern void func_0020EFDC(Vec3 *out, Vec3 *a, Vec3 *b);
extern f32 func_00298470(f32);
extern void func_0020EF60(Vec3 *out, Vec3 *in, f32 scale);
Vector4f *func_00235AD0(Vector4f *out, u32 bx, u32 by, u32 bz) {
    Vec3 axis;
    Vec3 cross;
    Vector4f result;
    f32 one;
    f32 magnitude;
    f32 angle;
    f32 trig;
    one = D_800A6508;
    axis.x = 0.0f;
    axis.y = one;
    axis.z = 0.0f;
    func_0020EFDC(&cross, &axis, (Vec3 *)&bx);
    magnitude = func_00298470((cross.x * cross.x) +
                              (cross.y * cross.y) +
                              (cross.z * cross.z));
    if (magnitude == 0.0f) {
        result.z = 0.0f;
        result.y = 0.0f;
        result.x = 0.0f;
        result.w = one;
    } else {
        func_0020EF60(&cross, &cross, one / magnitude);
        angle = func_0021153C((axis.x * *(f32 *)&bx) +
                              (axis.y * *(f32 *)&by) +
                              (axis.z * *(f32 *)&bz));
        angle *= ((struct func_802077F4_S2 *) ((f32 *) (&D_800A6508)))->unk4;
        trig = func_002982F0(angle);
        result.x = cross.x * trig;
        result.y = cross.y * trig;
        result.z = cross.z * trig;
        D_800DD78C = trig;
        result.w = func_002974C0(angle);
    }
    *out = result;
    return out;
}
