#include "common.h"
#include "structs/region.h"

/* Smallest and largest of a coordinate over a triangle's corners. */
#define TRI_MIN(t, c) ({                        \
    CVector3 *_p0 = (t)->m_pVertices[0];        \
    CVector3 *_p1 = (t)->m_pVertices[1];        \
    f32 _m = _p1->c;                            \
    f32 _n;                                     \
    if (!(_m <= _p0->c)) _m = _p0->c;           \
    _n = (t)->m_pVertices[2]->c;                \
    if (!(_n <= _m)) _n = _m;                   \
    _n;                                         \
})
#define TRI_MAX(t, c) ({                        \
    CVector3 *_p0 = (t)->m_pVertices[0];        \
    CVector3 *_p1 = (t)->m_pVertices[1];        \
    f32 _m = _p1->c;                            \
    f32 _n;                                     \
    if (!(_p0->c <= _m)) _m = _p0->c;           \
    _n = (t)->m_pVertices[2]->c;                \
    if (!(_m <= _n)) _n = _m;                   \
    _n;                                         \
})

/* Extent on the ground plane. */
typedef struct Rect {
    /* 0x00 */ f32 minX;
    /* 0x04 */ f32 minZ;
    /* 0x08 */ f32 maxX;
    /* 0x0C */ f32 maxZ;
} Rect;

s32 func_0026AFC0(CGameRegion *tri);   /* too steep to stand on */

/* tri's extent on the ground plane. */
static __inline__ void triRect(Rect *r, CGameRegion *tri) {
    r->minX = TRI_MIN(tri, x);
    r->maxX = TRI_MAX(tri, x);
    r->minZ = TRI_MIN(tri, z);
    r->maxZ = TRI_MAX(tri, z);
}

/* Top of tri, raised to that of any steep neighbour lying within its slightly grown footprint. */
f32 func_0026B130(CGameRegion *tri) {
    Rect box;
    Rect adjBox;
    CGameRegion *adj;
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
        adj = tri->m_pNeighbours[i];
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
