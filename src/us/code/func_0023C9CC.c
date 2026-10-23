/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/ANIM/802406C0_2.CPP, func_80240D20_de, with Turok 2's layouts. */
#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

/* A four-corner face: corners at 0x0..0x2C, normal at 0x30. */
typedef struct Quad {
    Vec3 corner[4]; /* 0x00 */
    Vec3 normal;    /* 0x30 */
} Quad;

void func_0020EF2C(Vec3 *out, Vec3 *a, Vec3 *b);
void func_0020EFDC(Vec3 *out, Vec3 *a, Vec3 *b);

/* The edges belong to this helper; declared in the face function, GCC grows the frame. */
static __inline__ void quad_normal(Quad *quad) {
    Vec3 edge1;
    Vec3 edge2;

    func_0020EF2C(&edge1, &quad->corner[1], &quad->corner[0]);
    func_0020EF2C(&edge2, &quad->corner[2], &quad->corner[1]);
    func_0020EFDC(&quad->normal, &edge1, &edge2);
}

/* Sets a box face from points 0, 1, 2 and 3 of the box and computes its (unnormalised) normal. */
void func_0023C9CC(Quad *quad, Vec3 *points) {
    quad->corner[0] = points[0];
    quad->corner[1] = points[1];
    quad->corner[2] = points[2];
    quad->corner[3] = points[3];
    quad_normal(quad);
}
