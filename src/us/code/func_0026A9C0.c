#include "structs/vector.h"
#include "globals/math.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AI/NAV/8027451C_2.CPP, func_80275410_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/region.h"
#include "globals/region.h"
extern f32 Math_AcosLookup(f32 arg0);
extern float func_002974C0(float x);
extern float func_002982F0(float x);
struct CQuatern_t;
typedef struct CQuatern_t CQuatern;
struct CQuatern_t;
struct CQuatern_t {
    float x;
    float y;
    float z;
    float w;
};
extern void CVector3__Normalize(f32 *arg0);
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
extern void CVector3__Cross(CVector3 *, CVector3 *, CVector3 *);
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
