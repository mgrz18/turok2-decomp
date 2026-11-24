#include "structs/vector.h"
#include "globals/region.h"
#include "protos/mtxf.h"
#include "structs/collision.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AI/NAV/8027451C.CPP, func_802750B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern void CVector3__Cross(CVector3 *, CVector3 *, CVector3 *);

static __inline__ CVector3 *cross_edges(CVector3 *out, CGameRegion *node) {
    CVector3 a;
    CVector3 b;
    char *p;
    char *q;
    if (node == 0) {
        ((CVector3 *)&sRegionCeilingNormal)->x = 0;
        ((CVector3 *)&sRegionCeilingNormal)->z = 0;
        ((CVector3 *)&sRegionCeilingNormal)->y = -1.0f;
    } else if (node != sCachedCeilingRawNormalRegion) {
        p = (char *)node->m_pVertices[1];
        q = (char *)node->m_pVertices[0];
        a.x = ((CollisionBuildVertex *)p)->x - ((CollisionBuildVertex *)q)->x;
        p = (char *)node->m_pVertices[1];
        q = (char *)node->m_pVertices[0];
        a.y = ((CollisionBuildVertex *)p)->ceiling - ((CollisionBuildVertex *)q)->ceiling;
        p = (char *)node->m_pVertices[1];
        q = (char *)node->m_pVertices[0];
        a.z = ((CollisionBuildVertex *)p)->z - ((CollisionBuildVertex *)q)->z;
        p = (char *)node->m_pVertices[2];
        q = (char *)node->m_pVertices[1];
        b.x = ((CollisionBuildVertex *)p)->x - ((CollisionBuildVertex *)q)->x;
        p = (char *)node->m_pVertices[2];
        q = (char *)node->m_pVertices[1];
        b.y = ((CollisionBuildVertex *)p)->ceiling - ((CollisionBuildVertex *)q)->ceiling;
        p = (char *)node->m_pVertices[2];
        q = (char *)node->m_pVertices[1];
        b.z = ((CollisionBuildVertex *)p)->z - ((CollisionBuildVertex *)q)->z;
        CVector3__Cross((CVector3 *)&sRegionCeilingNormal, &b, &a);
    }
    sCachedCeilingRawNormalRegion = node;
    *out = *(CVector3 *)&sRegionCeilingNormal;
    return out;
}

CVector3 *CGameRegion__GetCeilingNormal(CVector3 *out, CGameRegion *node) {
    if (node == 0) {
        ((CVector3 *)&sRegionCeilingUnitNormal)->x = 0;
        ((CVector3 *)&sRegionCeilingUnitNormal)->z = 0;
        ((CVector3 *)&sRegionCeilingUnitNormal)->y = -1.0f;
    } else if (node != sCachedCeilingNormalRegion) {
        cross_edges((CVector3 *)&sRegionCeilingUnitNormal, node);
        CVector3__Normalize((CVector3 *)&sRegionCeilingUnitNormal);
    }
    *out = *(CVector3 *)&sRegionCeilingUnitNormal;
    sCachedCeilingNormalRegion = node;
    return out;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de.c, func_8027525C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/level_file.h"
#include "structs/region.h"

f32 CGameRegion__GetCeilingHeight(CGameRegion *node, f32 x, f32 z) {
    CVector3 normal;
    CVector3 point;
    CVector3 a;
    CVector3 b;
    char *p;
    char *q;
    if (node == 0) {
        return 0.0f;
    }
    if (node != sCachedCeilingRawNormalRegion) {
        p = (char *)node->m_pVertices[1];
        q = (char *)node->m_pVertices[0];
        a.x = ((CollisionBuildVertex *)p)->x - ((CollisionBuildVertex *)q)->x;
        p = (char *)node->m_pVertices[1];
        q = (char *)node->m_pVertices[0];
        a.y = ((CollisionBuildVertex *)p)->ceiling - ((CollisionBuildVertex *)q)->ceiling;
        p = (char *)node->m_pVertices[1];
        q = (char *)node->m_pVertices[0];
        a.z = ((CollisionBuildVertex *)p)->z - ((CollisionBuildVertex *)q)->z;
        p = (char *)node->m_pVertices[2];
        q = (char *)node->m_pVertices[1];
        b.x = ((CollisionBuildVertex *)p)->x - ((CollisionBuildVertex *)q)->x;
        p = (char *)node->m_pVertices[2];
        q = (char *)node->m_pVertices[1];
        b.y = ((CollisionBuildVertex *)p)->ceiling - ((CollisionBuildVertex *)q)->ceiling;
        p = (char *)node->m_pVertices[2];
        q = (char *)node->m_pVertices[1];
        b.z = ((CollisionBuildVertex *)p)->z - ((CollisionBuildVertex *)q)->z;
        CVector3__Cross((CVector3 *)&sRegionCeilingNormal, &b, &a);
    }
    normal = *(CVector3 *)&sRegionCeilingNormal;
    sCachedCeilingRawNormalRegion = node;
    if (normal.y == 0.0f) {
        return (((CollisionBuildVertex *)node->m_pVertices[0])->ceiling +
                ((CollisionBuildVertex *)node->m_pVertices[1])->ceiling +
                ((CollisionBuildVertex *)node->m_pVertices[2])->ceiling) * 0.33333334f;
    }
    p = (char *)node->m_pVertices[0];
    point.x = ((CollisionBuildVertex *)p)->x;
    p = (char *)node->m_pVertices[0];
    point.y = ((CollisionBuildVertex *)p)->ceiling;
    p = (char *)node->m_pVertices[0];
    point.z = ((CollisionBuildVertex *)p)->z;
    return (((point.z - z) * normal.z) +
            ((point.x - x) * normal.x) + (point.y * normal.y)) /
           normal.y;
}

#include "common.h"

void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);    /* a - b */
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);    /* cross product */

/* tri's face normal, cached for the last triangle asked about. */
static __inline__ CVector3 triNormal(CGameRegion *tri) {
    CVector3 edge1;
    CVector3 edge2;

    if (tri != sCachedRawNormalRegion) {
        CVector3__Subtract(&edge1, tri->m_pVertices[1], tri->m_pVertices[0]);
        CVector3__Subtract(&edge2, tri->m_pVertices[2], tri->m_pVertices[1]);
        CVector3__Cross(&sRegionNormal, &edge1, &edge2);
    }
    sCachedRawNormalRegion = tri;
    return sRegionNormal;
}

/* The velocity something slides with down the slope of tri. */
CVector3 CGameRegion__GetSlideVelocity(CGameRegion *tri, f32 speed) {
    CVector3 normal;
    CVector3 up;
    CVector3 side;
    CVector3 downhill;
    CVector3 down;
    CVector3 result;

    if (tri == NULL) {
        result.x = result.y = result.z = 0.0f;
        return result;
    } else {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        normal = triNormal(tri);
        CVector3__Cross(&side, &normal, &up);
        CVector3__Cross(&downhill, &normal, &side);
        CVector3__Normalize(&downhill);
        if (downhill.y > 0.0f) {
            CVector3__Scale(&downhill, &downhill, -1.0f);
        }
        down.x = 0.0f;    /* set but never used */
        down.z = 0.0f;
        down.y = -1.0f;
        CVector3__Scale(&result, &downhill, -downhill.y * -speed * 10.24f);
    }
    return result;
}

f32 func_00298470(f32 x);                           /* sqrtf */

/* Height of tri's plane above (x, z). */
static __inline__ f32 triHeight(CGameRegion *tri, f32 x, f32 z) {
    CVector3 normal;
    CVector3 v;

    if (tri == NULL) {
        return -14336.0f;
    }
    normal = triNormal(tri);
    if (normal.y == 0.0f) {
        return (tri->m_pVertices[0]->y + tri->m_pVertices[1]->y + tri->m_pVertices[2]->y) * (1.0f / 3);
    }
    v = *tri->m_pVertices[0];
    return ((v.z - z) * normal.z + (v.x - x) * normal.x + v.y * normal.y) / normal.y;
}

/* Cosine of the slope climbed going from (x1, z1) to (x2, z2) on tri; 1 when not climbing. */
f32 CGameRegion__GetClimbCos(CGameRegion *tri, f32 x1, f32 z1, f32 x2, f32 z2) {
    f32 h1;
    f32 h2;
    f32 flat;
    f32 total;

    if (tri != NULL) {
        h1 = triHeight(tri, x1, z1);
        h2 = triHeight(tri, x2, z2);
        if (h1 < h2) {
            flat = (x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1);
            total = flat + (h2 - h1) * (h2 - h1);
            if (total != 0.0f) {
                return func_00298470(flat / total);
            }
        }
    }
    return 1.0f;
}

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

f32 Math_AcosLookup(f32 x);                           /* acosf */

/* Slope angle climbed going from (x1, z1) to (x2, z2) on tri; negative going down. */
f32 CGameRegion__GetClimbAngle(CGameRegion *tri, f32 x1, f32 z1, f32 x2, f32 z2) {
    f32 h1;
    f32 h2;
    f32 flat;
    f32 total;
    f32 angle;
    f32 cosine;

    if (tri == NULL) {
        return 0.0f;
    }
    h1 = triHeight(tri, x1, z1);
    h2 = triHeight(tri, x2, z2);
    flat = (x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1);
    total = flat + (h2 - h1) * (h2 - h1);
    angle = 0.0f;
    if (total != 0.0f) {
        cosine = func_00298470(flat / total);
        angle = Math_AcosLookup(MAX(-1.0f, MIN(cosine, 1.0f)));
        if (!(h1 < h2)) {
            angle = -angle;
        }
    }
    return angle;
}

/* tri's face normal, cached for the last triangle asked about; straight up without one. */
static __inline__ CVector3 triNormal_26A5A0(CGameRegion *tri) {
    CVector3 edge1;
    CVector3 edge2;

    if (tri == NULL) {
        sRegionNormal.x = 0.0f;
        sRegionNormal.z = 0.0f;
        sRegionNormal.y = 1.0f;
    } else if (tri != sCachedRawNormalRegion) {
        CVector3__Subtract(&edge1, tri->m_pVertices[1], tri->m_pVertices[0]);
        CVector3__Subtract(&edge2, tri->m_pVertices[2], tri->m_pVertices[1]);
        CVector3__Cross(&sRegionNormal, &edge1, &edge2);
    }
    sCachedRawNormalRegion = tri;
    return sRegionNormal;
}

/*
 * Where the segment from -> to crosses tri's plane, going against its normal:
 * *t gets the fraction along the segment (1 when moving away from the plane);
 * returns whether the crossing lies strictly between the ends.
 */
s32 CGameRegion__IntersectSegment(CGameRegion *tri, CVector3 *from, CVector3 *to, f32 *t) {
    CVector3 normal;
    CVector3 dir;
    CVector3 v;
    CVector3 diff;
    f32 denom;
    f32 frac;

    normal = triNormal_26A5A0(tri);
    CVector3__Subtract(&dir, to, from);
    if (tri != NULL) {
        v = *tri->m_pVertices[0];
    } else {
        v.x = v.y = v.z = 0.0f;
    }
    denom = normal.x * dir.x + normal.y * dir.y + normal.z * dir.z;
    if (denom >= 0.0f) {
        *t = 1.0f;
        return 0;
    }
    CVector3__Subtract(&diff, from, &v);
    frac = -(normal.x * diff.x + normal.y * diff.y + normal.z * diff.z) / denom;
    *t = frac;
    return frac > 0.0f && frac < 1.0f;
}

/* A vertex of this kind of triangle keeps its height after z. */
typedef struct Vertex {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 y;
} Vertex;
#define REGION_VERTEX(tri, i) ((Vertex *)(tri)->m_pVertices[i])

/* tri's face normal, cached for the last triangle asked about. */
static __inline__ CVector3 triNormal_26A798(CGameRegion *tri) {
    CVector3 edge1;
    CVector3 edge2;

    if (tri != sCachedCeilingRawNormalRegion) {
        edge1.x = REGION_VERTEX(tri, 1)->x - REGION_VERTEX(tri, 0)->x;
        edge1.y = REGION_VERTEX(tri, 1)->y - REGION_VERTEX(tri, 0)->y;
        edge1.z = REGION_VERTEX(tri, 1)->z - REGION_VERTEX(tri, 0)->z;
        edge2.x = REGION_VERTEX(tri, 2)->x - REGION_VERTEX(tri, 1)->x;
        edge2.y = REGION_VERTEX(tri, 2)->y - REGION_VERTEX(tri, 1)->y;
        edge2.z = REGION_VERTEX(tri, 2)->z - REGION_VERTEX(tri, 1)->z;
        CVector3__Cross(&sRegionCeilingNormal, &edge2, &edge1);
    }
    sCachedCeilingRawNormalRegion = tri;
    return sRegionCeilingNormal;
}

/*
 * Where the segment from -> to crosses tri's plane, going against its normal:
 * *t gets the fraction along the segment (1 when moving away from the plane);
 * returns whether the crossing lies strictly between the ends.
 */
s32 CGameRegion__IntersectSegmentCeiling(CGameRegion *tri, CVector3 *from, CVector3 *to, f32 *t) {
    CVector3 normal;
    CVector3 dir;
    CVector3 v;
    CVector3 diff;
    f32 denom;
    f32 frac;

    if (tri == NULL) {
        return 0;
    }
    normal = triNormal_26A798(tri);
    CVector3__Subtract(&dir, to, from);
    v.x = REGION_VERTEX(tri, 0)->x;
    v.y = REGION_VERTEX(tri, 0)->y;
    v.z = REGION_VERTEX(tri, 0)->z;
    denom = normal.x * dir.x + normal.y * dir.y + normal.z * dir.z;
    if (denom >= 0.0f) {
        *t = 1.0f;
        return 0;
    }
    CVector3__Subtract(&diff, from, &v);
    frac = -(normal.x * diff.x + normal.y * diff.y + normal.z * diff.z) / denom;
    *t = frac;
    return frac > 0.0f && frac < 1.0f;
}

#include "globals/math.h"

extern f32 Math_AcosLookup(f32 arg0);
extern float func_002974C0(float x);
extern float func_002982F0(float x);
struct CQuatern_t;
typedef struct CQuatern_t CQuatern;
struct CQuatern_t {
    float x;
    float y;
    float z;
    float w;
};
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
extern f32 func_00298470(f32);

static __inline__ void edge(CVector3 *out, CVector3 *a, CVector3 *b) {
    CVector3__Subtract(out, a, b);
}

static __inline__ void normalize_copy(CVector3 *out, CVector3 *in) {
    *out = *in;
    CVector3__Normalize(out);
}

extern void CVector3__Scale(CVector3 *, CVector3 *, float);

CQuatern *CGameRegion__GetTiltRotation(CQuatern *out, CGameRegion *polygon) {
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
        *(s32 *)&sRegionUnitNormal = 0;
        sRegionUnitNormal_z = 0;
        sRegionUnitNormal_y = 1.0f;
    } else if (polygon != sCachedNormalRegion) {
        if (polygon == 0) {
            *(s32 *)&sRegionNormal = 0;
            sRegionNormal_z = 0;
            sRegionNormal_y = 1.0f;
        } else if (polygon != sCachedRawNormalRegion) {
            edge(&edge0, polygon->m_pVertices[1], polygon->m_pVertices[0]);
            edge(&edge1, polygon->m_pVertices[2], polygon->m_pVertices[1]);
            CVector3__Cross(&sRegionNormal, &edge0, &edge1);
        }
        sCachedRawNormalRegion = polygon;
        normalize_copy(&sRegionUnitNormal, &sRegionNormal);
    }
    one = 1.0f;
    sCachedNormalRegion = polygon;
    normal = sRegionUnitNormal;
    up.x = 0.0f;
    up.y = one;
    up.z = 0.0f;
    CVector3__Cross(&axis, &up, &normal);
    len = func_00298470(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    if (len == 0.0f) {
        result.z = 0.0f;
        result.y = 0.0f;
        result.x = 0.0f;
        result.w = one;
    } else {
        CVector3__Scale(&axis, &axis, one / len);
        angle = Math_AcosLookup(up.x * normal.x + up.y * normal.y + up.z * normal.z);
        angle *= 0.5f;
        scale = func_002982F0(angle);
        result.x = axis.x * scale;
        result.y = axis.y * scale;
        result.z = axis.z * scale;
        gQuaternionSin = scale;
        result.w = func_002974C0(angle);
    }
    *out = result;
    return out;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de_2.c, func_80275688_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/region.h"

/* CGameRegion__GetCeilingNormal gets the region this function was given (its only caller,
 * CParticle__GetParentTransform, passes it): the ROM leaves $a1 as it came, which is the same code
 * as passing it (until round 28 the N64 build called it with one argument, docs/port/ABI.md). */
Vector4f *CGameRegion__GetCeilingTiltRotation(Vector4f *out, void *region) {
    CVector3 a;
    CVector3 up;
    CVector3 axis;
    Vector4f result;
    float len;
    float angle;
    float scale;
    float one;
    CGameRegion__GetCeilingNormal(&a, region);
    one = 1.0f;
    up.x = 0;
    up.y = one;
    up.z = 0;
    CVector3__Cross(&axis, &up, &a);
    len = func_00298470(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    if (len == 0) {
        result.z = 0;
        result.y = 0;
        result.x = 0;
        result.w = one;
    } else {
        CVector3__Scale(&axis, &axis, one / len);
        angle = Math_AcosLookup(up.x * a.x + up.y * a.y + up.z * a.z);
        angle *= 0.5f;
        scale = func_002982F0(angle);
        result.x = axis.x * scale;
        result.y = axis.y * scale;
        result.z = axis.z * scale;
        gQuaternionSin = scale;
        result.w = func_002974C0(angle);
    }
    *out = result;
    return out;
}

f32 func_002982F0(f32 x);   /* sinf */
f32 func_002974C0(f32 x);   /* cosf */
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);    /* quaternion product */

/* Heading of tri's edge from vertex i to the next one, seen from above. */
static __inline__ f32 edgeHeading(CGameRegion *tri, s32 i) {
    CVector3 *a = tri->m_pVertices[i];
    CVector3 *b = tri->m_pVertices[(i + 1) % 3];
    f32 dx = b->x - a->x;
    f32 dz = b->z - a->z;
    f32 len = func_00298470(dx * dx + dz * dz);
    f32 angle;

    if (len == 0.0f) {
        return 0.0f;
    }
    angle = Math_AcosLookup(dx / len);
    if (!(dz < 0.0f)) {
        angle = -angle;
    }
    return angle;
}

/* Orientation lying along tri's edge i: turned to its heading, then tipped a quarter turn. */
CQuaternion CGameRegion__GetEdgeRotation(CGameRegion *tri, s32 i) {
    CQuaternion heading;
    CQuaternion tilt;
    CQuaternion result;
    f32 s;
    f32 half;

    {   /* its own temporary: a function-wide one costs a register copy */
        f32 t = func_002982F0(edgeHeading(tri, i) * 0.5f);
        heading.x = 0.0f;
        heading.y = t;
        heading.z = 0.0f;
        gQuaternionSin = t;
    }
    heading.w = func_002974C0(edgeHeading(tri, i) * 0.5f);
    half = -0.785398245f;
    s = func_002982F0(half);
    gQuaternionSin = s;
    tilt.x = s;
    tilt.y = 0.0f;
    tilt.z = 0.0f;
    tilt.w = func_002974C0(half);
    CQuaternion__Multiply(&result, &tilt, &heading);
    return result;
}

s32 CGameRegion__IsSteep(CGameRegion *polygon) {
    CVector3 up;
    CVector3 normal;
    CVector3 edge0;
    CVector3 edge1;
    if (polygon == 0) {
        return 0;
    }
    if (!(polygon->m_wFlags & 0x2000)) {
        return polygon->m_wFlags & 8;
    }
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    if (polygon != sCachedNormalRegion) {
        if (polygon == 0) {
            *(s32 *)&sRegionNormal = 0;
            sRegionNormal_y = 1.0f;
            sRegionNormal_z = 0;
        } else if (polygon != sCachedRawNormalRegion) {
            edge(&edge0, polygon->m_pVertices[1], polygon->m_pVertices[0]);
            edge(&edge1, polygon->m_pVertices[2], polygon->m_pVertices[1]);
            CVector3__Cross(&sRegionNormal, &edge0, &edge1);
        }
        sCachedRawNormalRegion = polygon;
        normalize_copy(&sRegionUnitNormal, &sRegionNormal);
    }
    normal = sRegionUnitNormal;
    sCachedNormalRegion = polygon;
    if (normal.x * up.x + normal.y * up.y + normal.z * up.z <= 0.5f) {
        return 1;
    }
    return 0;
}

#include "structs/rect.h"
/* Smallest and largest of a coordinate over a triangle's corners. */
#define TRI_MIN(t, c) ({                        \
    CVector3 *_p0 = (t)->m_pVertices[0];                      \
    CVector3 *_p1 = (t)->m_pVertices[1];                      \
    f32 _m = _p1->c;                            \
    f32 _n;                                     \
    if (!(_m <= _p0->c)) _m = _p0->c;           \
    _n = (t)->m_pVertices[2]->c;                \
    if (!(_n <= _m)) _n = _m;                   \
    _n;                                         \
})
#define TRI_MAX(t, c) ({                        \
    CVector3 *_p0 = (t)->m_pVertices[0];                      \
    CVector3 *_p1 = (t)->m_pVertices[1];                      \
    f32 _m = _p1->c;                            \
    f32 _n;                                     \
    if (!(_p0->c <= _m)) _m = _p0->c;           \
    _n = (t)->m_pVertices[2]->c;                \
    if (!(_m <= _n)) _n = _m;                   \
    _n;                                         \
})

/* Extent on the ground plane. */

/* tri's extent on the ground plane. */
static __inline__ void triRect(CRect *r, CGameRegion *tri) {
    r->minX = TRI_MIN(tri, x);
    r->maxX = TRI_MAX(tri, x);
    r->minZ = TRI_MIN(tri, z);
    r->maxZ = TRI_MAX(tri, z);
}

/* Top of tri, raised to that of any steep neighbour lying within its slightly grown footprint. */
f32 CGameRegion__GetTopWithSteepNeighbours(CGameRegion *tri) {
    CRect box;
    CRect adjBox;
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
        if (adj == NULL || !CGameRegion__IsSteep(adj)) {
            continue;
        }
        triRect(&adjBox, adj);
        if (adjBox.maxX < box.maxX && box.minX < adjBox.minX && adjBox.maxZ < box.maxZ && box.minZ < adjBox.minZ) {
            top = TRI_MAX(adj, y) < top ? top : TRI_MAX(adj, y);
        }
    }
    return top;
}

#include "structs/world.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/iset.h"

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

static __inline__ void cross_edges_26B4A4(CVector3 *out, CGameRegion *tri) {
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
        cross_edges_26B4A4(&sRegionNormal, tri);
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

void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);    /* a + b */

/* Centre of tri, as the mean of its edges' midpoints. */
static __inline__ void triCentre(CVector3 *out, CGameRegion *tri) {
    CVector3 mid0;
    CVector3 mid1;
    CVector3 mid2;

    CVector3__Add(&mid0, tri->m_pVertices[0], tri->m_pVertices[1]);
    CVector3__Scale(&mid0, &mid0, 0.5f);
    CVector3__Add(&mid1, tri->m_pVertices[1], tri->m_pVertices[2]);
    CVector3__Scale(&mid1, &mid1, 0.5f);
    CVector3__Add(&mid2, tri->m_pVertices[2], tri->m_pVertices[0]);
    CVector3__Scale(&mid2, &mid2, 0.5f);
    CVector3__Add(out, &mid0, &mid1);
    CVector3__Add(out, out, &mid2);
    CVector3__Scale(out, out, 1.0f / 3);
}

/* Distance between the centres of two triangles. */
f32 CGameRegion__GetCentreDistance(CGameRegion *a, CGameRegion *b) {
    CVector3 diff;
    CVector3 centreA;
    CVector3 centreB;

    triCentre(&centreA, a);
    triCentre(&centreB, b);
    CVector3__Subtract(&diff, &centreA, &centreB);
    return func_00298470(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}

void BoundingBox_FromPoints(f32 *arg0, s32 arg1, CVector3 *arg2)
{
    s32 i;
    i = 0;
    if (arg1 != 0) {
        arg0[0] = arg2->x;
        arg0[1] = arg2->y;
        arg0[2] = arg2->z;
        arg0[3] = arg2->x;
        arg0[4] = arg2->y;
        arg1 -= 1;
        arg0[5] = arg2->z;
        arg2 += 1;
        if (arg1 > 0) {
bounds_loop:
                {
                    f32 value = arg2->x;
                    f32 bound = arg0[0];
                    if (!(value <= bound)) {
                        value = bound;
                    }
                    arg0[0] = value;
                }
                {
                    f32 value = arg2->y;
                    f32 bound = arg0[1];
                    if (!(value <= bound)) {
                        value = bound;
                    }
                    arg0[1] = value;
                }
                {
                    f32 value = arg2->z;
                    f32 bound = arg0[2];
                    if (!(value <= bound)) {
                        value = bound;
                    }
                    arg0[2] = value;
                }
                {
                    f32 value = arg2->x;
                    f32 bound = arg0[3];
                    if (!(bound <= value)) {
                        value = bound;
                    }
                    arg0[3] = value;
                }
                {
                    f32 value = arg2->y;
                    f32 bound = arg0[4];
                    if (!(bound <= value)) {
                        value = bound;
                    }
                    arg0[4] = value;
                }
                {
                    f32 value = arg2->z;
                    f32 bound = arg0[5];
                    if (!(bound <= value)) {
                        value = bound;
                    }
                    arg0[5] = value;
                }
                i += 1;
                arg2 += 1;
            if (i < arg1) goto bounds_loop;
        }
    }
}
void UnpackScaledVec4(void *arg0);

void UnpackScaledVec4(void *arg0) {
    s32 *ip = (s32 *)arg0;
    f32 *fp = (f32 *)arg0;
    f32 scale = 0.029296875f;
    fp[0] = (f32)ip[0] * scale;
    fp[1] = (f32)ip[1] * scale;
    fp[2] = (f32)ip[2] * scale;
    fp[3] = (f32)ip[3] * scale;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de_4.c, func_80275AA4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifdef TARGET_PORT
/* The port: the same through CGameRegion (structs/region.h), whose host size is not the 32 bytes
 * the neighbour index is scaled by; vertices stay 16 bytes (CollisionBuildVertex). */
#include "common.h"
#include "structs/region.h"

typedef struct {
    /* 0x00 */ u16 info;
    /* 0x02 */ u16 flags;
    /* 0x04 */ u16 vertex[3];
    /* 0x0A */ u16 neighbour[3];
    /* 0x10 */ s32 visBits;
} PackedRegion; /* size 0x14 */

void CGameRegion__Unpack(CGameRegion *dst, PackedRegion *src, char *base, int unused, CGameRegion *extra) {
    int i;
    dst->m_nInfo = src->info;
    dst->m_wFlags = src->flags;
    dst->m_VisBits = src->visBits;
    for (i = 0; i < 3; i++) {
        dst->m_pVertices[i] = (CVector3 *)(base + src->vertex[i] * 16);
        dst->m_pNeighbours[i] = src->neighbour[i] == 0xFFFF ? NULL : &extra[src->neighbour[i]];
    }
}
#else
#include "rw/abu_types.h"

extern void CGameRegion__Unpack(HeaderRecAt1C *dst, Packed *src, char *base, int unused, char *extra);
void CGameRegion__Unpack(HeaderRecAt1C *dst, Packed *src, char *base, int unused, char *extra) {
    int i;
    dst->unk0 = src->unk0;
    dst->unk2 = src->unk2;
    dst->unk1C = src->unk10;
    for (i = 0; i < 3; i++) {
        dst->first[i] = base + src->first[i] * 16;
        if (src->second[i] == 0xFFFF) {
            dst->second[i] = 0;
        } else {
            dst->second[i] = extra + src->second[i] * 32;
        }
    }
}
#endif

typedef struct {
    f32 x, y, z;
} Vtx3;
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vtx3 *verts[3];
} Tri;

/* Is (x, z) inside the XZ projection of the triangle? */
s32 CGameRegion__ContainsXZ(Tri *tri, f32 x, f32 z) {
    s32 i, next;
    Vtx3 *a, *b;

    if (tri == NULL) {
        return 1;
    }
    for (i = 0; i < 3; i = next) {
        next = i + 1;
        a = tri->verts[i];
        b = tri->verts[next % 3];
        if ((b->z - a->z) * (x - a->x) + (a->x - b->x) * (z - a->z) < 0.0f) {
            return 0;
        }
    }
    return 1;
}

/* The port (ABI): every caller passes the result's address first (the MIPS hidden result
 * pointer of a struct return; CGameRegion__GetCeilingNormalRaw is written that way); x86-64
 * returns a 12-byte struct in registers, so the callers' region arrived as the result pointer. */
CVector3 *CGameRegion__GetNormalRaw(CVector3 *out, CGameRegion *polygon) {
    CVector3 edge0;
    CVector3 edge1;

    if (polygon == NULL) {
        sRegionNormal.x = 0.0f;
        sRegionNormal.z = 0.0f;
        sRegionNormal.y = 1.0f;
    } else if (polygon != sCachedRawNormalRegion) {
        CVector3__Subtract(&edge0, polygon->m_pVertices[1], polygon->m_pVertices[0]);
        CVector3__Subtract(&edge1, polygon->m_pVertices[2], polygon->m_pVertices[1]);
        CVector3__Cross(&sRegionNormal, &edge0, &edge1);
    }
    sCachedRawNormalRegion = polygon;
    *out = sRegionNormal;
    return out;
}

/* Raw (unnormalised) normal of a triangle, cached for the last triangle. */
static __inline__ CVector3 *raw_normal(CVector3 *out, CGameRegion *tri) {
    CVector3 edge0;
    CVector3 edge1;

    if (tri == NULL) {
        sRegionNormal.x = 0;
        sRegionNormal.z = 0;
        sRegionNormal.y = 1.0f;
    } else if (tri != sCachedRawNormalRegion) {
        CVector3__Subtract(&edge0, tri->m_pVertices[1], tri->m_pVertices[0]);
        CVector3__Subtract(&edge1, tri->m_pVertices[2], tri->m_pVertices[1]);
        CVector3__Cross(&sRegionNormal, &edge0, &edge1);
    }
    sCachedRawNormalRegion = tri;
    *out = sRegionNormal;
    return out;
}

/* Unit normal of a triangle (straight up for none), cached for the last triangle. */
CVector3 *CGameRegion__GetNormal(CVector3 *out, CGameRegion *tri) {
    if (tri == NULL) {
        sRegionUnitNormal.x = 0;
        sRegionUnitNormal.z = 0;
        sRegionUnitNormal.y = 1.0f;
    } else if (tri != sCachedNormalRegion) {
        raw_normal(&sRegionUnitNormal, tri);
        CVector3__Normalize(&sRegionUnitNormal);
    }
    *out = sRegionUnitNormal;
    sCachedNormalRegion = tri;
    return out;
}

static __inline__ void cross_edges_26BE60(CVector3 *out, CGameRegion *tri) {
    CVector3 edge0;
    CVector3 edge1;

    CVector3__Subtract(&edge0, tri->m_pVertices[1], tri->m_pVertices[0]);
    CVector3__Subtract(&edge1, tri->m_pVertices[2], tri->m_pVertices[1]);
    CVector3__Cross(out, &edge0, &edge1);
}

/* Height of a triangle's plane at (x, z); the mean vertex height for a vertical one. */
f32 CGameRegion__GetHeight(CGameRegion *tri, f32 x, f32 z) {
    CVector3 normal;
    CVector3 point;

    if (tri == NULL) {
        return -14336.0f;
    }
    if (tri != sCachedRawNormalRegion) {
        cross_edges_26BE60(&sRegionNormal, tri);
    }
    normal = sRegionNormal;
    sCachedRawNormalRegion = tri;
    if (normal.y == 0.0f) {
        return (tri->m_pVertices[0]->y + tri->m_pVertices[1]->y + tri->m_pVertices[2]->y) * (1.0f / 3);
    }
    point = *tri->m_pVertices[0];
    return ((point.z - z) * normal.z + (point.x - x) * normal.x + point.y * normal.y) / normal.y;
}

/* A collision vertex with a floor and a ceiling height. */
typedef struct {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 floor;
    /* 0x8 */ f32 z;
    /* 0xC */ f32 ceiling;
} CeilingVertex;
/* A region's vertex seen with its ceiling height. */
#define CEILING_VERTEX(tri, i) ((CeilingVertex *)(tri)->m_pVertices[i])

/* Raw normal of a triangle's ceiling plane (straight down for none), cached for the last triangle. */
CVector3 *CGameRegion__GetCeilingNormalRaw(CVector3 *out, CGameRegion *tri) {
    CVector3 a;
    CVector3 b;

    if (tri == NULL) {
        sRegionCeilingNormal.x = 0;
        sRegionCeilingNormal.z = 0;
        sRegionCeilingNormal.y = -1.0f;
    } else if (tri != sCachedCeilingRawNormalRegion) {
        a.x = CEILING_VERTEX(tri, 1)->x - CEILING_VERTEX(tri, 0)->x;
        a.y = CEILING_VERTEX(tri, 1)->ceiling - CEILING_VERTEX(tri, 0)->ceiling;
        a.z = CEILING_VERTEX(tri, 1)->z - CEILING_VERTEX(tri, 0)->z;
        b.x = CEILING_VERTEX(tri, 2)->x - CEILING_VERTEX(tri, 1)->x;
        b.y = CEILING_VERTEX(tri, 2)->ceiling - CEILING_VERTEX(tri, 1)->ceiling;
        b.z = CEILING_VERTEX(tri, 2)->z - CEILING_VERTEX(tri, 1)->z;
        CVector3__Cross(&sRegionCeilingNormal, &b, &a);
    }
    *out = sRegionCeilingNormal;
    sCachedCeilingRawNormalRegion = tri;
    return out;
}

#include "protos/scene.h"

u8 CGameRegion__GetFloorSurface(sptr arg0) {
    CRegionInfo *temp_v0;

    temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
    if (temp_v0 == NULL) {
        return 0U;
    }
    return temp_v0->unk_58;
}

u8 CGameRegion__GetWallSurface(sptr arg0) {
    CRegionInfo *temp_v0;

    temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
    if (temp_v0 == NULL) {
        return 0U;
    }
    return temp_v0->unk_59[0];
}

/* Whether the segment from a to b crosses the height of the region's surface,
 * strictly between its ends; *t gets the crossing fraction.
 * The failure paths share the first `return 0` through a label: the ROM's
 * single `v0 = 0; j epilogue` block sits right after the region test, where
 * three separate returns would be cross-jumped into the last one instead. */
s32 CGameRegion__IntersectWater(CGameRegion *region, CVector3 *a, CVector3 *b, f32 *t) {
    CRegionInfo *info;
    f32 dy;
    f32 f;
    s32 result;

    if (region == NULL || !(region->m_wFlags & 1)) {
    fail:
        return 0;
    }
    if (region != sCachedRegionInfoRegion) {
        sCachedRegionInfo = CScene__GetRegionInfo(&gScene, region);
        sCachedRegionInfoRegion = region;
    }
    info = sCachedRegionInfo;
    if (info == NULL) {
        goto fail;
    }
    dy = b->y - a->y;
    if (dy == 0.0f) {
        goto fail;
    }
    result = 0;
    f = (info->m_WaterHeight - a->y) / dy;
    *t = f;
    if (f > 0.0f && f < 1.0f) {
        result = 1;
    }
    return result;
}

f32 CGameRegion__GetEdgeYaw(void *arg0, s32 arg1) {
    Triangle *polygon = arg0;
    CVector3 *v0;
    CVector3 *v1;
    f32 dx, dz;
    f32 mag;
    f32 result;
    v1 = polygon->vertices[arg1];
    v0 = polygon->vertices[(arg1 + 1) % 3];
    dx = v0->x - v1->x;
    dz = v0->z - v1->z;
    mag = func_00298470((dx * dx) + (dz * dz));
    if (mag == 0.0f) {
        return 0.0f;
    }
    result = Math_AcosLookup(dx / mag);
    if (!(dz < 0.0f)) {
        result = -result;
    }
    return result;
}

extern f32 Math_AcosLookup(f32);                                  /* acosf */
extern void CMtxF__TransformPoint(f32 mtx[4][4], CVector3 *in, CVector3 *out); /* out = mtx * in */

/* Yaw of a triangle edge (CGameRegion__GetEdgeYaw). */
static __inline__ f32 edge_yaw(CGameRegion *tri, s32 edge) {
    CVector3 *v0;
    CVector3 *v1;
    f32 dx, dz;
    f32 mag;
    f32 result;

    v1 = tri->m_pVertices[edge];
    v0 = tri->m_pVertices[(edge + 1) % 3];
    dx = v0->x - v1->x;
    dz = v0->z - v1->z;
    mag = func_00298470(dx * dx + dz * dz);
    if (mag == 0.0f) {
        return 0.0f;
    }
    result = Math_AcosLookup(dx / mag);
    if (!(dz < 0.0f)) {
        result = -result;
    }
    return result;
}

/* Horizontal unit vector facing along a triangle edge's yaw. */
CVector3 *CGameRegion__GetEdgeDirection(CVector3 *out, CGameRegion *tri, s32 edge) {
    CVector3 dir;
    CVector3 forward;
    f32 mtx[4][4];

    CMtxF__SetRotateY(mtx, edge_yaw(tri, edge));
    forward.x = 0;
    forward.y = 0;
    forward.z = 1.0f;
    CMtxF__TransformPoint(mtx, &forward, &dir);
    *out = dir;
    return out;
}

/* Raw (unnormalised) normal of a triangle, cached for the last triangle. */
static __inline__ CVector3 raw_normal_26C434(CGameRegion *tri) {
    CVector3 edge0;
    CVector3 edge1;

    if (tri == NULL) {
        sRegionNormal.x = 0;
        sRegionNormal.z = 0;
        sRegionNormal.y = 1.0f;
    } else if (tri != sCachedRawNormalRegion) {
        CVector3__Subtract(&edge0, tri->m_pVertices[1], tri->m_pVertices[0]);
        CVector3__Subtract(&edge1, tri->m_pVertices[2], tri->m_pVertices[1]);
        CVector3__Cross(&sRegionNormal, &edge0, &edge1);
    }
    sCachedRawNormalRegion = tri;
    return sRegionNormal;
}

/* Downhill yaw of a sloped triangle; 0 when it is not a slope or is level. */
f32 CGameRegion__GetDownhillYaw(CGameRegion *tri) {
    CVector3 normal;
    f32 mag;
    f32 result;

    if (!CGameRegion__IsSteep(tri)) {
        return 0.0f;
    }
    normal = raw_normal_26C434(tri);
    mag = func_00298470(normal.x * normal.x + normal.z * normal.z);
    if (mag == 0.0f) {
        return 0.0f;
    }
    result = Math_AcosLookup(normal.z / mag);
    if (!(normal.x > 0.0f)) {
        result = -result;
    }
    return result;
}

void CGameRegion__FloodSetFlag4(CGameRegion *arg0) {
    if ((arg0 != NULL) && (arg0->m_wFlags & 2)) {
        CGameRegion__FloodSetFlag4Same(arg0, arg0->m_nInfo);
    }
}

void CGameRegion__FloodClearFlag4(CGameRegion *arg0) {
    if ((arg0 != NULL) && (arg0->m_wFlags & 2)) {
        CGameRegion__FloodClearFlag4Same(arg0, arg0->m_nInfo);
    }
}

/* Resource entry: a key, its flags and three child entries. */



void CGameRegion__FloodSetWater(CGameRegion *arg0) {
    sptr temp_a0;
    sptr temp_a0_2;
    sptr temp_a0_3;
    CRegionInfo *temp_v0;
    s32 temp_s0;
    u16 temp_v1;

    if (arg0 != NULL) {
        temp_v1 = arg0->m_wFlags;
        if (!(temp_v1 & 1)) {
            temp_a0 = (sptr) arg0->m_pNeighbours[0];
            temp_s0 = arg0->m_nInfo;
            arg0->m_wFlags = (u16) (temp_v1 | 1);
            if (temp_a0 != 0) {
                CGameRegion__FloodSetWaterSame((CGameRegion *)temp_a0, temp_s0);
            }
            temp_a0_2 = (sptr) arg0->m_pNeighbours[1];
            if (temp_a0_2 != 0) {
                CGameRegion__FloodSetWaterSame((CGameRegion *)temp_a0_2, temp_s0);
            }
            temp_a0_3 = (sptr) arg0->m_pNeighbours[2];
            if (temp_a0_3 != 0) {
                CGameRegion__FloodSetWaterSame((CGameRegion *)temp_a0_3, temp_s0);
            }
        }
        temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
        if (temp_v0 != NULL) {
            temp_v0->m_MapColour = 2;
            temp_v0->m_dwFlags = temp_v0->m_dwFlags | 1;
            temp_v0->unk_5B = (u8) (temp_v0->unk_5B | 2);
        }
    }
}

void CGameRegion__FloodClearWater(CGameRegion *arg0) {
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;
    u16 temp_v1;
    CRegionInfo *temp_v0;

    if (arg0 != NULL) {
        temp_v1 = arg0->m_wFlags;
        if (temp_v1 & 1) {
            temp_a0 = arg0->m_pNeighbours[0];
            arg0->m_wFlags = (u16) (temp_v1 & 0xFFFE);
            if (temp_a0 != NULL) {
                CGameRegion__FloodClearWaterAll(temp_a0);
            }
            temp_a0_2 = arg0->m_pNeighbours[1];
            if (temp_a0_2 != NULL) {
                CGameRegion__FloodClearWaterAll(temp_a0_2);
            }
            temp_a0_3 = arg0->m_pNeighbours[2];
            if (temp_a0_3 != NULL) {
                CGameRegion__FloodClearWaterAll(temp_a0_3);
            }
        }
        temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
        if (temp_v0 != NULL) {
            temp_v0->unk_58 = 0;
            temp_v0->m_MapColour = 0;
            temp_v0->m_dwFlags = (s32) (temp_v0->m_dwFlags & ~1);
            temp_v0->unk_5B = (u8) (temp_v0->unk_5B & 0xFD);
        }
    }
}

void CGameRegion__SetFlag80000(s32 arg0) {
    CRegionInfo *temp_v0;

    temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
    if (temp_v0 != NULL) {
        temp_v0->m_dwFlags = temp_v0->m_dwFlags | 0x80000;
    }
}

void CGameRegion__ClearFlag80000(s32 arg0) {
    CRegionInfo *temp_v0;

    temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
    if (temp_v0 != NULL) {
        temp_v0->m_dwFlags = (s32) (temp_v0->m_dwFlags & 0xFFF7FFFF);
    }
}

/* A 0x64-byte record of the world model sections (CScene__GetRegionInfo finds it from an id). */


void CGameRegion__SetFlag4000(u16 *arg0) {
    CRegionInfo *temp_v0;

    temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
    if (temp_v0 != NULL) {
        temp_v0->m_dwFlags |= 0x4000;
    }
    if (arg0 != NULL) {
        CGameRegion__FloodSetFlag4000Same((CGameRegion *)arg0, *arg0);
    }
}

void CGameRegion__SetFlag40000(s32 arg0) {
    CRegionInfo *temp_v0;

    temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
    if (temp_v0 != NULL) {
        temp_v0->m_dwFlags |= 0x40000;
    }
}

void CGameRegion__ClearFlag40000(s32 arg0) {
    CRegionInfo *temp_v0;

    temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
    if (temp_v0 != NULL) {
        temp_v0->m_dwFlags = (s32) (temp_v0->m_dwFlags & 0xFFFBFFFF);
    }
}

#include "m2c_macros.h"

f32 CGameRegion__SetGroupHeight(u16 *arg0, f32 arg1) {
    f32 var_f0;

    var_f0 = arg1;
    if (arg0 != NULL) {
#ifdef TARGET_PORT
        CGameRegion__FloodSetHeight((CGameRegion *)arg0, arg1, *arg0);   /* returns void; no caller of CGameRegion__SetGroupHeight uses its result */
#else
        var_f0 = ((f32 (*)(CGameRegion *, f32, u16))CGameRegion__FloodSetHeight)((CGameRegion *)arg0, arg1, *arg0);   /* the ROM reads the $f0 it leaves */
#endif
    }
    return var_f0;
}

/* Marks arg0 and, recursively, every node linked to it with id arg1. */
void CGameRegion__FloodSetFlag4Same(CGameRegion *arg0, u16 arg1) {
    u16 temp_s0;
    u16 temp_v1;
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    temp_s0 = arg0->m_nInfo;
    if (temp_s0 == (arg1 & 0xFFFF)) {
        temp_v1 = arg0->m_wFlags;
        if (!(temp_v1 & 4)) {
            temp_a0 = arg0->m_pNeighbours[0];
            arg0->m_wFlags = (u16) (temp_v1 | 4);
            if (temp_a0 != NULL) {
                CGameRegion__FloodSetFlag4Same(temp_a0, temp_s0);
            }
            temp_a0_2 = arg0->m_pNeighbours[1];
            if (temp_a0_2 != NULL) {
                CGameRegion__FloodSetFlag4Same(temp_a0_2, temp_s0);
            }
            temp_a0_3 = arg0->m_pNeighbours[2];
            if (temp_a0_3 != NULL) {
                CGameRegion__FloodSetFlag4Same(temp_a0_3, temp_s0);
            }
        }
    }
}

/* Resource entry: a key, its flags and three child entries. */

void CGameRegion__FloodClearFlag4Same(CGameRegion *arg0, u16 arg1) {
    u16 temp_s0;
    u16 temp_v1;
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    temp_s0 = arg0->m_nInfo;
    if (temp_s0 == (arg1 & 0xFFFF)) {
        temp_v1 = arg0->m_wFlags;
        if (temp_v1 & 4) {
            temp_a0 = arg0->m_pNeighbours[0];
            arg0->m_wFlags = (u16) (temp_v1 & 0xFFFB);
            if (temp_a0 != NULL) {
                CGameRegion__FloodClearFlag4Same(temp_a0, temp_s0);
            }
            temp_a0_2 = arg0->m_pNeighbours[1];
            if (temp_a0_2 != NULL) {
                CGameRegion__FloodClearFlag4Same(temp_a0_2, temp_s0);
            }
            temp_a0_3 = arg0->m_pNeighbours[2];
            if (temp_a0_3 != NULL) {
                CGameRegion__FloodClearFlag4Same(temp_a0_3, temp_s0);
            }
        }
    }
}

/* Marks (0x4000) every node of the subtree that has the given id. */
void CGameRegion__FloodSetFlag4000Same(CGameRegion *arg0, u16 arg1) {
    u16 temp_s0;
    u16 temp_v1;
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    temp_s0 = arg0->m_nInfo;
    if (temp_s0 == (arg1 & 0xFFFF)) {
        temp_v1 = arg0->m_wFlags;
        if (!(temp_v1 & 0x4000)) {
            temp_a0 = arg0->m_pNeighbours[0];
            arg0->m_wFlags = (u16) (temp_v1 | 0x4000);
            if (temp_a0 != NULL) {
                CGameRegion__FloodSetFlag4000Same(temp_a0, temp_s0);
            }
            temp_a0_2 = arg0->m_pNeighbours[1];
            if (temp_a0_2 != NULL) {
                CGameRegion__FloodSetFlag4000Same(temp_a0_2, temp_s0);
            }
            temp_a0_3 = arg0->m_pNeighbours[2];
            if (temp_a0_3 != NULL) {
                CGameRegion__FloodSetFlag4000Same(temp_a0_3, temp_s0);
            }
        }
    }
}

/* Sets the height of arg0's vertices to arg1 and spreads it to every node
 * with id arg2 linked to it. */
void CGameRegion__FloodSetHeight(CGameRegion *arg0, f32 arg1, s32 arg2) {
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    if (arg0->m_nInfo == (arg2 & 0xFFFF)) {
        if (arg0->m_pVertices[0]->y == arg1) {
            if ((arg0->m_pVertices[1]->y != arg1) || (arg0->m_pVertices[2]->y != arg1)) {
                goto block_5;
            }
        } else {
block_5:
            arg0->m_pVertices[0]->y = arg1;
            arg0->m_pVertices[1]->y = arg1;
            arg0->m_pVertices[2]->y = arg1;
            temp_a0 = arg0->m_pNeighbours[0];
            if (temp_a0 != NULL) {
                CGameRegion__FloodSetHeight(temp_a0, arg1, arg2 & 0xFFFF);
            }
            temp_a0_2 = arg0->m_pNeighbours[1];
            if (temp_a0_2 != NULL) {
                CGameRegion__FloodSetHeight(temp_a0_2, arg1, arg2 & 0xFFFF);
            }
            temp_a0_3 = arg0->m_pNeighbours[2];
            if (temp_a0_3 != NULL) {
                CGameRegion__FloodSetHeight(temp_a0_3, arg1, arg2 & 0xFFFF);
            }
        }
    }
}

void CVector3__Add(CVector3 *, CVector3 *, CVector3 *);

/* The centroid of the region's triangle, as the mean of its three edge midpoints. */
void CGameRegion__GetCentroid(CGameRegion *region, CVector3 *out) {
    CVector3 sp10;
    CVector3 sp20;
    CVector3 sp30;
    f32 half;

    CVector3__Add(&sp10, region->m_pVertices[0], region->m_pVertices[1]);
    half = 0.5f;
    CVector3__Scale(&sp10, &sp10, half);
    CVector3__Add(&sp20, region->m_pVertices[1], region->m_pVertices[2]);
    CVector3__Scale(&sp20, &sp20, half);
    CVector3__Add(&sp30, region->m_pVertices[2], region->m_pVertices[0]);
    CVector3__Scale(&sp30, &sp30, half);
    CVector3__Add(out, &sp10, &sp20);
    CVector3__Add(out, out, &sp30);
    CVector3__Scale(out, out, 0.33333334f);
}

/* Clears the 0x8000 mark on arg0 and every marked node it links to. */
void CGameRegion__ClearVisited(CGameRegion *arg0) {
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    temp_a0 = arg0->m_pNeighbours[0];
    arg0->m_wFlags = (u16) (arg0->m_wFlags & 0x7FFF);
    if ((temp_a0 != NULL) && (temp_a0->m_wFlags & 0x8000)) {
        CGameRegion__ClearVisited(temp_a0);
    }
    temp_a0_2 = arg0->m_pNeighbours[1];
    if ((temp_a0_2 != NULL) && (temp_a0_2->m_wFlags & 0x8000)) {
        CGameRegion__ClearVisited(temp_a0_2);
    }
    temp_a0_3 = arg0->m_pNeighbours[2];
    if ((temp_a0_3 != NULL) && (temp_a0_3->m_wFlags & 0x8000)) {
        CGameRegion__ClearVisited(temp_a0_3);
    }
}

void func_0026CCA0(void) {

}
void BoundingBox_AddPoints(f32 *arg0, s32 arg1, CVector3 *arg2);

void BoundingBox_AddPoints(f32 *arg0, s32 arg1, CVector3 *arg2)
{
    s32 i;
    i = 0;
    if (arg1 > 0) {
        do {
            {
                f32 value = arg2[i].x;
                f32 bound = arg0[0];
                if (!(value <= bound)) {
                    value = bound;
                }
                arg0[0] = value;
            }
            {
                f32 value = arg2[i].y;
                f32 bound = arg0[1];
                if (!(value <= bound)) {
                    value = bound;
                }
                arg0[1] = value;
            }
            {
                f32 value = arg2[i].z;
                f32 bound = arg0[2];
                if (!(value <= bound)) {
                    value = bound;
                }
                arg0[2] = value;
            }
            {
                f32 value = arg2[i].x;
                f32 bound = arg0[3];
                if (!(bound <= value)) {
                    value = bound;
                }
                arg0[3] = value;
            }
            {
                f32 value = arg2[i].y;
                f32 bound = arg0[4];
                if (!(bound <= value)) {
                    value = bound;
                }
                arg0[4] = value;
            }
            {
                f32 value = arg2[i].z;
                f32 bound = arg0[5];
                if (!(bound <= value)) {
                    value = bound;
                }
                arg0[5] = value;
            }
            i += 1;
        } while (i < arg1);
    }
}

#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ f32 minX;
    /* 0x04 */ f32 minZ;
    /* 0x08 */ f32 maxX;
    /* 0x0C */ f32 maxZ;
} Bounds2D;
void BoundingRect_AddPoints(f32 *arg0, s32 arg1, CVector3 *arg2);

/* Starts the x/z bounds at the first point, then grows them over the rest. */
void BoundingRect_FromPoints(Bounds2D *arg0, s32 arg1, CVector3 *arg2) {
    f32 temp_f0;
    f32 temp_f0_2;

    if (arg1 != 0) {
        temp_f0 = arg2->x;
        arg0->maxX = temp_f0;
        arg0->minX = temp_f0;
        temp_f0_2 = arg2->z;
        arg0->maxZ = temp_f0_2;
        arg0->minZ = temp_f0_2;
        BoundingRect_AddPoints((sptr) arg0, arg1 - 1, (sptr) (arg2 + 1));
    }
}

void BoundingRect_AddPoints(f32 *arg0, s32 arg1, CVector3 *arg2)
{
    f32 value;
    f32 bound;
    s32 i;
    i = 0;
    if (arg1 > 0) {
        do {
            value = arg2[i].x;
            bound = arg0[0];
            if (!(value <= bound)) {
                value = bound;
            }
            arg0[0] = value;
            value = arg2[i].z;
            bound = arg0[1];
            if (!(value <= bound)) {
                value = bound;
            }
            arg0[1] = value;
            value = arg2[i].x;
            bound = arg0[2];
            if (!(bound <= value)) {
                value = bound;
            }
            arg0[2] = value;
            value = arg2[i].z;
            bound = arg0[3];
            if (!(bound <= value)) {
                value = bound;
            }
            arg0[3] = value;
            i += 1;
        } while (i < arg1);
    }
}

void func_0026CE3C(void) {

}

void CGameRegion__FloodSetWaterSame(CGameRegion *arg0, u16 arg1) {
    u16 temp_s0;
    u16 temp_v1;
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    temp_s0 = arg0->m_nInfo;
    if (temp_s0 == (arg1 & 0xFFFF)) {
        temp_v1 = arg0->m_wFlags;
        if (!(temp_v1 & 1)) {
            temp_a0 = arg0->m_pNeighbours[0];
            arg0->m_wFlags = temp_v1 | 1;
            if (temp_a0 != NULL) {
                CGameRegion__FloodSetWaterSame(temp_a0, temp_s0);
            }
            temp_a0_2 = arg0->m_pNeighbours[1];
            if (temp_a0_2 != NULL) {
                CGameRegion__FloodSetWaterSame(temp_a0_2, temp_s0);
            }
            temp_a0_3 = arg0->m_pNeighbours[2];
            if (temp_a0_3 != NULL) {
                CGameRegion__FloodSetWaterSame(temp_a0_3, temp_s0);
            }
        }
    }
}

/* Clears flag 1 over the subtree. */
void CGameRegion__FloodClearWaterAll(CGameRegion *arg0) {
    u16 temp_v1;
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    temp_v1 = arg0->m_wFlags;
    if (temp_v1 & 1) {
        temp_a0 = arg0->m_pNeighbours[0];
        arg0->m_wFlags = (u16) (temp_v1 & 0xFFFE);
        if (temp_a0 != NULL) {
            CGameRegion__FloodClearWaterAll(temp_a0);
        }
        temp_a0_2 = arg0->m_pNeighbours[1];
        if (temp_a0_2 != NULL) {
            CGameRegion__FloodClearWaterAll(temp_a0_2);
        }
        temp_a0_3 = arg0->m_pNeighbours[2];
        if (temp_a0_3 != NULL) {
            CGameRegion__FloodClearWaterAll(temp_a0_3);
        }
    }
}

/* The XZ bounding rectangle of the triangle. */
void CGameRegion__GetRect(CGameRegion *arg0, CRect *arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 var_f0;
    f32 var_f0_2;
    f32 var_f0_3;
    f32 var_f0_4;
    f32 var_f1;
    f32 var_f1_2;
    f32 var_f1_3;
    f32 var_f1_4;

    var_f1 = arg0->m_pVertices[1]->x;
    temp_f0 = arg0->m_pVertices[0]->x;
    if (!(var_f1 <= temp_f0)) {
        var_f1 = temp_f0;
    }
    var_f0 = arg0->m_pVertices[2]->x;
    if (!(var_f0 <= var_f1)) {
        var_f0 = var_f1;
    }
    arg1->minX = var_f0;
    var_f1_2 = arg0->m_pVertices[1]->x;
    temp_f0_2 = arg0->m_pVertices[0]->x;
    if (!(temp_f0_2 <= var_f1_2)) {
        var_f1_2 = temp_f0_2;
    }
    var_f0_2 = arg0->m_pVertices[2]->x;
    if (!(var_f1_2 <= var_f0_2)) {
        var_f0_2 = var_f1_2;
    }
    arg1->maxX = var_f0_2;
    var_f1_3 = arg0->m_pVertices[1]->z;
    temp_f0_3 = arg0->m_pVertices[0]->z;
    if (!(var_f1_3 <= temp_f0_3)) {
        var_f1_3 = temp_f0_3;
    }
    var_f0_3 = arg0->m_pVertices[2]->z;
    if (!(var_f0_3 <= var_f1_3)) {
        var_f0_3 = var_f1_3;
    }
    arg1->minZ = var_f0_3;
    var_f1_4 = arg0->m_pVertices[1]->z;
    temp_f0_4 = arg0->m_pVertices[0]->z;
    if (!(temp_f0_4 <= var_f1_4)) {
        var_f1_4 = temp_f0_4;
    }
    var_f0_4 = arg0->m_pVertices[2]->z;
    if (!(var_f1_4 <= var_f0_4)) {
        var_f0_4 = var_f1_4;
    }
    arg1->maxZ = var_f0_4;
}

/* The highest y of the triangle's three vertices. */
f32 CGameRegion__GetMaxHeight(CGameRegion *arg0) {
    f32 temp_f0;
    f32 var_f0;
    f32 var_f1;

    var_f1 = arg0->m_pVertices[1]->y;
    temp_f0 = arg0->m_pVertices[0]->y;
    if (!(temp_f0 <= var_f1)) {
        var_f1 = temp_f0;
    }
    var_f0 = arg0->m_pVertices[2]->y;
    if (!(var_f1 <= var_f0)) {
        var_f0 = var_f1;
    }
    return var_f0;
}
