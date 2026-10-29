#include "common.h"

typedef struct {
    f32 x, y, z;
} CVector3;

typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u16 flags;
    /* 0x04 */ CVector3 *vertices[3];
} Triangle;

extern Triangle *D_800B2608; /* triangle whose normal is cached */
extern CVector3 D_800F2020;  /* cached (unnormalized) normal */

void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);

/* Unnormalized normal of a triangle (up when there is none), cached. */
CVector3 func_0026BC54(Triangle *polygon) {
    CVector3 edge0;
    CVector3 edge1;

    if (polygon == NULL) {
        D_800F2020.x = 0.0f;
        D_800F2020.z = 0.0f;
        D_800F2020.y = 1.0f;
    } else if (polygon != D_800B2608) {
        func_0020EF2C(&edge0, polygon->vertices[1], polygon->vertices[0]);
        func_0020EF2C(&edge1, polygon->vertices[2], polygon->vertices[1]);
        func_0020EFDC(&D_800F2020, &edge0, &edge1);
    }
    D_800B2608 = polygon;
    return D_800F2020;
}
