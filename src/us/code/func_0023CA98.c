/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/ANIM/802406C0_2.CPP, func_80240D20_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);

/* The edges belong to this helper; declared in the face function, GCC grows the frame. */
static __inline__ void quad_normal(CollisionQuad *quad) {
    CVector3 edge1;
    CVector3 edge2;

    func_0020EF2C(&edge1, &quad->corner[1], &quad->corner[0]);
    func_0020EF2C(&edge2, &quad->corner[2], &quad->corner[1]);
    func_0020EFDC(&quad->normal, &edge1, &edge2);
}

/* Sets a box face from points 0, 3, 7 and 4 of the box and computes its (unnormalised) normal. */
void func_0023CA98(CollisionQuad *quad, CVector3 *points) {
    quad->corner[0] = points[0];
    quad->corner[1] = points[3];
    quad->corner[2] = points[7];
    quad->corner[3] = points[4];
    quad_normal(quad);
}
