/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AI/NAV/8027451C.CPP, cross_edges, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

/* A collision vertex with a floor and a ceiling height. */
typedef struct {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 floor;
    /* 0x8 */ f32 z;
    /* 0xC */ f32 ceiling;
} CeilingVertex;

typedef struct {
    /* 0x0 */ u8 pad0[2];
    /* 0x2 */ u16 flags;
    /* 0x4 */ CeilingVertex *vertices[3];
} Triangle;

extern void func_0020EFDC(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a x b */

extern Triangle *D_800B2600;    /* triangle of the cached ceiling normal */
extern Vec3f D_800F2000;        /* cached raw ceiling normal */

/* Raw normal of a triangle's ceiling plane (straight down for none), cached for the last triangle. */
Vec3f *func_0026BFB4(Vec3f *out, Triangle *tri) {
    Vec3f a;
    Vec3f b;

    if (tri == NULL) {
        D_800F2000.x = 0;
        D_800F2000.z = 0;
        D_800F2000.y = -1.0f;
    } else if (tri != D_800B2600) {
        a.x = tri->vertices[1]->x - tri->vertices[0]->x;
        a.y = tri->vertices[1]->ceiling - tri->vertices[0]->ceiling;
        a.z = tri->vertices[1]->z - tri->vertices[0]->z;
        b.x = tri->vertices[2]->x - tri->vertices[1]->x;
        b.y = tri->vertices[2]->ceiling - tri->vertices[1]->ceiling;
        b.z = tri->vertices[2]->z - tri->vertices[1]->z;
        func_0020EFDC(&D_800F2000, &b, &a);
    }
    *out = D_800F2000;
    D_800B2600 = tri;
    return out;
}
