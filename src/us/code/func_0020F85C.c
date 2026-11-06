#include "common.h"
#include "structs/game_object.h"

void CMtxF__TransformPoint(CMtxF mf, CVector3 *in, CVector3 *out) {
    out->x = (f32) ((mf[0][0] * in->x) + (mf[1][0] * in->y) + (mf[2][0] * in->z) + mf[3][0]);
    out->y = (f32) ((mf[0][1] * in->x) + (mf[1][1] * in->y) + (mf[2][1] * in->z) + mf[3][1]);
    out->z = (f32) ((mf[0][2] * in->x) + (mf[1][2] * in->y) + (mf[2][2] * in->z) + mf[3][2]);
}
