#include "common.h"
#include "structs/game_object.h"

/* out = the 3x3 part of the matrix m (rows) times v. */
void CMtxF__InverseRotateVector(f32 m[4][4], CVector3 *v, CVector3 *out) {
    out->x = (m[0][0] * v->x) + (m[0][1] * v->y) + (m[0][2] * v->z);
    out->y = (m[1][0] * v->x) + (m[1][1] * v->y) + (m[1][2] * v->z);
    out->z = (m[2][0] * v->x) + (m[2][1] * v->y) + (m[2][2] * v->z);
}
