/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AI/NAV/8027451C_2.CPP, func_80275410_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
extern f32 func_0021153C(f32 arg0);
struct CVector3_t;
extern float func_002974C0(float x);
extern float func_002982F0(float x);
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
struct CQuatern_t;
typedef struct CQuatern_t CQuatern;
struct CQuatern_t;
struct CQuatern_t {
    float x;
    float y;
    float z;
    float w;
};
struct Triangle;
struct Triangle {
    char pad0[2];
    u16 flags;
    CVector3 *vertices[3];
};
struct Triangle;
typedef struct Triangle Triangle;
extern void func_0020F040(f32 *arg0);
extern Triangle *D_800B2608;
extern Triangle *D_800B260C;
extern CVector3 D_800F2020;
extern f32 D_800F2024;
extern s32 D_800F2028;
extern CVector3 D_800F2030;
extern f32 D_800F2034;
extern s32 D_800F2038;
extern f32 D_800DD78C;
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
extern void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);
extern void func_0020EF60(CVector3 *out, CVector3 *in, f32 scale);
extern f32 func_00298470(f32);
static __inline__ void edge(CVector3 *out, CVector3 *a, CVector3 *b) {
    func_0020EF2C(out, a, b);
}
static __inline__ void normalize_copy(CVector3 *out, CVector3 *in) {
    *out = *in;
    func_0020F040(out);
}
extern float D_800DD78C;
extern void func_0020EFDC(CVector3 *, CVector3 *, CVector3 *);
extern void func_0020EF60(CVector3 *, CVector3 *, float);
CQuatern *func_0026A9C0(CQuatern *out, Triangle *polygon) {
    CVector3 normal;
    CVector3 up;
    CVector3 axis;
    CQuatern result;
    CVector3 edge0;
    CVector3 edge1;
    f32 one;
    f32 len;
    f32 angle;
    f32 scale;
    if (polygon == 0) {
        *(s32 *)&D_800F2030 = 0;
        D_800F2038 = 0;
        D_800F2034 = 1.0f;
    } else if (polygon != D_800B260C) {
        if (polygon == 0) {
            *(s32 *)&D_800F2020 = 0;
            D_800F2028 = 0;
            D_800F2024 = 1.0f;
        } else if (polygon != D_800B2608) {
            edge(&edge0, polygon->vertices[1], polygon->vertices[0]);
            edge(&edge1, polygon->vertices[2], polygon->vertices[1]);
            func_0020EFDC(&D_800F2020, &edge0, &edge1);
        }
        D_800B2608 = polygon;
        normalize_copy(&D_800F2030, &D_800F2020);
    }
    one = 1.0f;
    D_800B260C = polygon;
    normal = D_800F2030;
    up.x = 0.0f;
    up.y = one;
    up.z = 0.0f;
    func_0020EFDC(&axis, &up, &normal);
    len = func_00298470(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    if (len == 0.0f) {
        result.z = 0.0f;
        result.y = 0.0f;
        result.x = 0.0f;
        result.w = one;
    } else {
        func_0020EF60(&axis, &axis, one / len);
        angle = func_0021153C(up.x * normal.x + up.y * normal.y + up.z * normal.z);
        angle *= 0.5f;
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
