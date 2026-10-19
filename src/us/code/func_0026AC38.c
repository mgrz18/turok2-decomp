/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de_2.c, func_80275688_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 func_0021153C(f32 arg0);
extern float func_002974C0(float x);
extern float func_002982F0(float x);
extern float D_800A7FC4;
extern f32 D_800DD78C;
extern void func_0020EFDC(Vec3 *out, Vec3 *a, Vec3 *b);
extern void func_0020EF60(Vec3 *out, Vec3 *in, f32 scale);
extern f32 func_00298470(f32);
extern float D_800A7FBC[];
extern float D_800DD78C;
extern void func_00269A90(Vec3 *);
extern void func_0020EFDC(Vec3 *, Vec3 *, Vec3 *);
extern void func_0020EF60(Vec3 *, Vec3 *, float);
Vector4f *func_0026AC38(Vector4f *out) {
    Vec3 a;
    Vec3 up;
    Vec3 axis;
    Vector4f result;
    float len;
    float angle;
    float scale;
    float one;
    func_00269A90(&a);
    one = D_800A7FBC[1];
    up.x = 0;
    up.y = one;
    up.z = 0;
    func_0020EFDC(&axis, &up, &a);
    len = func_00298470(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    if (len == 0) {
        result.z = 0;
        result.y = 0;
        result.x = 0;
        result.w = one;
    } else {
        func_0020EF60(&axis, &axis, one / len);
        angle = func_0021153C(up.x * a.x + up.y * a.y + up.z * a.z);
        angle *= D_800A7FC4;
        scale = func_002982F0(angle);
        result.x = axis.x * scale;
        result.y = axis.y * scale;
        result.z = axis.z * scale;
        D_800DD78C = scale;
        result.w = func_002974C0(angle);
    }
    *out = result;
    return out;
}
