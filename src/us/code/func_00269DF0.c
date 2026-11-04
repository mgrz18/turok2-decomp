#include "common.h"
#include "structs/vector.h"

typedef struct Triangle {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ CVector3 *v[3];
} Triangle;

extern Triangle *D_800B2608;    /* triangle whose normal is cached */
extern CVector3 D_800F2020;         /* its normal */

void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);    /* a - b */
void func_0020EF60(CVector3 *out, CVector3 *in, f32 scale);
void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);    /* cross product */
void func_0020F040(CVector3 *v);                        /* normalize */

/* tri's face normal, cached for the last triangle asked about. */
static __inline__ CVector3 triNormal(Triangle *tri) {
    CVector3 edge1;
    CVector3 edge2;

    if (tri != D_800B2608) {
        func_0020EF2C(&edge1, tri->v[1], tri->v[0]);
        func_0020EF2C(&edge2, tri->v[2], tri->v[1]);
        func_0020EFDC(&D_800F2020, &edge1, &edge2);
    }
    D_800B2608 = tri;
    return D_800F2020;
}

/* The velocity something slides with down the slope of tri. */
CVector3 func_00269DF0(Triangle *tri, f32 speed) {
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
        func_0020EFDC(&side, &normal, &up);
        func_0020EFDC(&downhill, &normal, &side);
        func_0020F040(&downhill);
        if (downhill.y > 0.0f) {
            func_0020EF60(&downhill, &downhill, -1.0f);
        }
        down.x = 0.0f;    /* set but never used */
        down.z = 0.0f;
        down.y = -1.0f;
        func_0020EF60(&result, &downhill, -downhill.y * -speed * 10.24f);
    }
    return result;
}
