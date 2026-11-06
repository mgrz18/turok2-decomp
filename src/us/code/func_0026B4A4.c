#include "common.h"
#include "structs/region.h"
#include "structs/world.h"

typedef struct {
    /* 0x00 */ f32 minX;
    /* 0x04 */ f32 minZ;
    /* 0x08 */ f32 maxX;
    /* 0x0C */ f32 maxZ;
    /* 0x10 */ u32 count; /* triangles in this group, consecutive in the list */
} TriGroup; /* 0x14 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 count;
    /* 0x08 */ TriGroup groups[1];
} TriGroupList;

extern s32 gPersistentDataLoaded;          /* level loaded */
extern CScene gScene;
extern CGameRegion *sCachedRawNormalRegion;    /* triangle of the cached raw normal */
extern CVector3 sRegionNormal;        /* cached raw normal */

extern void *CISet__GetBlock(void *file, s32 index); /* a block of a file */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a x b */

/* CGameRegion__ContainsXZ, inlined: is (x, z) inside the XZ projection of the triangle? */
static __inline__ s32 InTriangle(CGameRegion *tri, f32 x, f32 z) {
    s32 i, next;
    CVector3 *a, *b;

    if (tri == NULL) {
        return 1;
    }
    for (i = 0; i < 3; i = next) {
        next = i + 1;
        a = tri->m_pVertices[i];
        b = tri->m_pVertices[next % 3];
        if ((b->z - a->z) * (x - a->x) + (a->x - b->x) * (z - a->z) < 0.0f) {
            return 0;
        }
    }
    return 1;
}

static __inline__ void cross_edges(CVector3 *out, CGameRegion *tri) {
    CVector3 edge0;
    CVector3 edge1;

    CVector3__Subtract(&edge0, tri->m_pVertices[1], tri->m_pVertices[0]);
    CVector3__Subtract(&edge1, tri->m_pVertices[2], tri->m_pVertices[1]);
    CVector3__Cross(out, &edge0, &edge1);
}

/* CGameRegion__GetHeight, inlined: height of a triangle's plane at (x, z). */
static __inline__ f32 TriHeight(CGameRegion *tri, f32 x, f32 z) {
    CVector3 normal;
    CVector3 point;

    if (tri == NULL) {
        return -14336.0f;
    }
    if (tri != sCachedRawNormalRegion) {
        cross_edges(&sRegionNormal, tri);
    }
    normal = sRegionNormal;
    sCachedRawNormalRegion = tri;
    if (normal.y == 0.0f) {
        return (tri->m_pVertices[0]->y + tri->m_pVertices[1]->y + tri->m_pVertices[2]->y) * (1.0f / 3);
    }
    point = *tri->m_pVertices[0];
    return ((point.z - z) * normal.z + (point.x - x) * normal.x + point.y * normal.y) / normal.y;
}

/* The level triangle nearest below pos (other than skip), NULL if none. */
CGameRegion *CGameRegion__FindBelow(CGameRegion *skip, CVector3 pos) {
    CScene *world;
    CGameRegion *tris;
    TriGroupList *list;
    TriGroup *groups;
    TriGroup *group;
    CGameRegion *tri;
    CGameRegion *best;
    f32 bestDist;
    f32 h;
    f32 d;
    s32 index;
    s32 n;
    s32 i;
    u32 j;

    if (!gPersistentDataLoaded) {
        return NULL;
    }
    world = &gScene;
    tris = (CGameRegion *) ((u8 *) CISet__GetBlock(world->m_pRegionData, 2) + 8);
    list = CISet__GetBlock(world->m_pRegionData, 3);
    n = list->count;
    groups = list->groups;
    best = NULL;
    bestDist = 0.0f;
    index = 0;
    for (i = 0; i < n; i++) {
        group = &groups[i];
        if (pos.x >= group->minX && group->maxX >= pos.x && pos.z >= group->minZ && group->maxZ >= pos.z) {
            for (j = 0; j < group->count; j++) {
                tri = &tris[index];
                if (InTriangle(tri, pos.x, pos.z) && tri != skip) {
                    h = TriHeight(tri, pos.x, pos.z);
                    if (h < pos.y) {
                        d = pos.y - h;
                        if (best == NULL) {
                            best = tri;
                            bestDist = d;
                        } else if (d < bestDist) {
                            best = tri;
                            bestDist = d;
                        }
                    }
                }
                index++;
            }
        } else {
            index += group->count;
        }
    }
    return best;
}
