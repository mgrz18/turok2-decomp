/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AI/NAV/8027451C.CPP, func_802750B0_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x0 */ u8 pad0[2];
    /* 0x2 */ u16 flags;
    /* 0x4 */ Vec3f *vertices[3];
} Triangle;

extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a - b */
extern void func_0020EFDC(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a x b */
extern void func_0020F040(Vec3f *v);                         /* normalise */

extern Triangle *D_800B2608;    /* triangle of the cached raw normal */
extern Triangle *D_800B260C;    /* triangle of the cached unit normal */
extern Vec3f D_800F2020;        /* cached raw normal */
extern Vec3f D_800F2030;        /* cached unit normal */

/* Raw (unnormalised) normal of a triangle, cached for the last triangle. */
static __inline__ Vec3f *raw_normal(Vec3f *out, Triangle *tri) {
    Vec3f edge0;
    Vec3f edge1;

    if (tri == NULL) {
        D_800F2020.x = 0;
        D_800F2020.z = 0;
        D_800F2020.y = 1.0f;
    } else if (tri != D_800B2608) {
        func_0020EF2C(&edge0, tri->vertices[1], tri->vertices[0]);
        func_0020EF2C(&edge1, tri->vertices[2], tri->vertices[1]);
        func_0020EFDC(&D_800F2020, &edge0, &edge1);
    }
    D_800B2608 = tri;
    *out = D_800F2020;
    return out;
}

/* Unit normal of a triangle (straight up for none), cached for the last triangle. */
Vec3f *func_0026BD20(Vec3f *out, Triangle *tri) {
    if (tri == NULL) {
        D_800F2030.x = 0;
        D_800F2030.z = 0;
        D_800F2030.y = 1.0f;
    } else if (tri != D_800B260C) {
        raw_normal(&D_800F2030, tri);
        func_0020F040(&D_800F2030);
    }
    *out = D_800F2030;
    D_800B260C = tri;
    return out;
}
