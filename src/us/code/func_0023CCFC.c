/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/ANIM/802406C0_2.CPP, func_80240D20_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);

/* The edges belong to this helper; declared in the face function, GCC grows the frame. */
static __inline__ void quad_normal(CollisionQuad *quad) {
    CVector3 edge1;
    CVector3 edge2;

    CVector3__Subtract(&edge1, &quad->corner[1], &quad->corner[0]);
    CVector3__Subtract(&edge2, &quad->corner[2], &quad->corner[1]);
    CVector3__Cross(&quad->normal, &edge1, &edge2);
}

/* Sets a box face from points 4, 5, 1 and 0 of the box and computes its (unnormalised) normal. */
void Quad_SetBoxSide4510(CollisionQuad *quad, CVector3 *points) {
    quad->corner[0] = points[4];
    quad->corner[1] = points[5];
    quad->corner[2] = points[1];
    quad->corner[3] = points[0];
    quad_normal(quad);
}
