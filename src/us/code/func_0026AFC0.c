/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AI/NAV/8027451C_3.CPP, func_802757E4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
struct CVector3_t;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
extern float D_800A7FD8;
struct Triangle;
struct Triangle {
    char pad0[2];
    u16 flags;
    CVector3 *vertices[3];
};
struct Triangle;
typedef struct Triangle Triangle;
extern float D_800A7FD0;
extern void func_0020F040(f32 *arg0);
extern Triangle *D_800B2608;
extern Triangle *D_800B260C;
extern CVector3 D_800F2020;
extern f32 D_800F2024;
extern s32 D_800F2028;
extern CVector3 D_800F2030;
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
extern void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);
static __inline__ void edge(CVector3 *out, CVector3 *a, CVector3 *b) {
    func_0020EF2C(out, a, b);
}
static __inline__ void normalize_copy(CVector3 *out, CVector3 *in) {
    *out = *in;
    func_0020F040(out);
}
s32 func_0026AFC0(Triangle *polygon) {
    CVector3 up;
    CVector3 normal;
    CVector3 edge0;
    CVector3 edge1;
    if (polygon == 0) {
        return 0;
    }
    if (!(polygon->flags & 0x2000)) {
        return polygon->flags & 8;
    }
    up.x = 0.0f;
    up.y = *(&D_800A7FD0 + 1);
    up.z = 0.0f;
    if (polygon != D_800B260C) {
        if (polygon == 0) {
            *(s32 *)&D_800F2020 = 0;
            D_800F2024 = *(&D_800A7FD0 + 1);
            D_800F2028 = 0;
        } else if (polygon != D_800B2608) {
            edge(&edge0, polygon->vertices[1], polygon->vertices[0]);
            edge(&edge1, polygon->vertices[2], polygon->vertices[1]);
            func_0020EFDC(&D_800F2020, &edge0, &edge1);
        }
        D_800B2608 = polygon;
        normalize_copy(&D_800F2030, &D_800F2020);
    }
    normal = D_800F2030;
    D_800B260C = polygon;
    if (normal.x * up.x + normal.y * up.y + normal.z * up.z <= D_800A7FD8) {
        return 1;
    }
    return 0;
}
