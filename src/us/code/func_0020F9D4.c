#include "common.h"
#include "structs/game_object.h"

void CMtxF__TransformPointProjective(CMtxF mf, CVector3 *in, CVector3 *out) {
    f32 temp_f1;
    f32 temp_f2;

    out->x = (f32) ((mf[0][0] * in->x) + (mf[1][0] * in->y) + (mf[2][0] * in->z) + mf[3][0]);
    out->y = (f32) ((mf[0][1] * in->x) + (mf[1][1] * in->y) + (mf[2][1] * in->z) + mf[3][1]);
    out->z = (f32) ((mf[0][2] * in->x) + (mf[1][2] * in->y) + (mf[2][2] * in->z) + mf[3][2]);
    temp_f2 = (mf[0][3] * in->x) + (mf[1][3] * in->y) + (mf[2][3] * in->z) + mf[3][3];
    if (temp_f2 != 0.0f) {
        temp_f1 = 1.0f / temp_f2;
        out->x = (f32) (out->x * temp_f1);
        out->y = (f32) (out->y * temp_f1);
        out->z = (f32) (out->z * temp_f1);
    }
}
