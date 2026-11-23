#include "common.h"
#include "structs/game_object.h"

/* arg2 = arg1 rotated by the transpose of arg0's 3x3 part. */
void CMtxF__RotateVector(CMtxF arg0, CVector3 *arg1, CVector3 *arg2) {
    arg2->x = (f32) ((arg0[0][0] * arg1->x) + (arg0[1][0] * arg1->y) + (arg0[2][0] * arg1->z));
    arg2->y = (f32) ((arg0[0][1] * arg1->x) + (arg0[1][1] * arg1->y) + (arg0[2][1] * arg1->z));
    arg2->z = (f32) ((arg0[0][2] * arg1->x) + (arg0[1][2] * arg1->y) + (arg0[2][2] * arg1->z));
}
