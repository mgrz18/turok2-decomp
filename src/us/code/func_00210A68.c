/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80273B98_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
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
struct Mtx;
struct Mtx {
    Vec3 right;
    f32 m03;
    Vec3 up;
    f32 m13;
    Vec3 forward;
    f32 m23;
    f32 m30;
    f32 m31;
    f32 m32;
    f32 m33;
};
struct Mtx;
typedef struct Mtx Mtx;
void func_00210A68(void *arg0, f32 arg1) {
    struct Mtx *m = (char *) arg0;
    f32 sin_v;
    f32 cos_v;
    f32 neg_sin;
    f32 a;
    sin_v = func_002982F0(arg1);
    cos_v = func_002974C0(arg1);
    neg_sin = -sin_v;
    a = m->right.x;
    m->right.x = (cos_v * a) + (sin_v * m->up.x);
    m->up.x = (neg_sin * a) + (cos_v * m->up.x);
    a = m->right.y;
    m->right.y = (cos_v * a) + (sin_v * m->up.y);
    m->up.y = (neg_sin * a) + (cos_v * m->up.y);
    a = m->right.z;
    m->right.z = (cos_v * a) + (sin_v * m->up.z);
    m->up.z = (neg_sin * a) + (cos_v * m->up.z);
}
