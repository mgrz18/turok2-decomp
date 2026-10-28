#include "common.h"

/* Smallest and largest of a coordinate over a triangle's corners. */
#define TRI_MIN(t, c) ({                        \
    Vec3 *_p0 = (t)->v[0];                      \
    Vec3 *_p1 = (t)->v[1];                      \
    f32 _m = _p1->c;                            \
    f32 _n;                                     \
    if (!(_m <= _p0->c)) _m = _p0->c;           \
    _n = (t)->v[2]->c;                          \
    if (!(_n <= _m)) _n = _m;                   \
    _n;                                         \
})
#define TRI_MAX(t, c) ({                        \
    Vec3 *_p0 = (t)->v[0];                      \
    Vec3 *_p1 = (t)->v[1];                      \
    f32 _m = _p1->c;                            \
    f32 _n;                                     \
    if (!(_p0->c <= _m)) _m = _p0->c;           \
    _n = (t)->v[2]->c;                          \
    if (!(_m <= _n)) _n = _m;                   \
    _n;                                         \
})

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Triangle {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 *v[3];
    /* 0x10 */ struct Triangle *adj[3];    /* across each edge */
} Triangle;

/* Extent on the ground plane. */
typedef struct Rect {
    /* 0x00 */ f32 minX;
    /* 0x04 */ f32 minZ;
    /* 0x08 */ f32 maxX;
    /* 0x0C */ f32 maxZ;
} Rect;

s32 func_0026AFC0(Triangle *tri);   /* too steep to stand on */

/* tri's extent on the ground plane. */
static __inline__ void triRect(Rect *r, Triangle *tri) {
    r->minX = TRI_MIN(tri, x);
    r->maxX = TRI_MAX(tri, x);
    r->minZ = TRI_MIN(tri, z);
    r->maxZ = TRI_MAX(tri, z);
}

/* Top of tri, raised to that of any steep neighbour lying within its slightly grown footprint. */
f32 func_0026B130(Triangle *tri) {
    Rect box;
    Rect adjBox;
    Triangle *adj;
    f32 top;
    f32 margin;
    s32 i;

    top = TRI_MAX(tri, y);
    triRect(&box, tri);
    margin = 5.12f;
    box.minX -= margin;
    box.maxX += margin;
    box.minZ -= margin;
    box.maxZ += margin;
    for (i = 0; i < 3; i++) {
        adj = tri->adj[i];
        if (adj == NULL || !func_0026AFC0(adj)) {
            continue;
        }
        triRect(&adjBox, adj);
        if (adjBox.maxX < box.maxX && box.minX < adjBox.minX && adjBox.maxZ < box.maxZ && box.minZ < adjBox.minZ) {
            top = TRI_MAX(adj, y) < top ? top : TRI_MAX(adj, y);
        }
    }
    return top;
}
