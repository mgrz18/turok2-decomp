#include "common.h"
#include "structs/game_object.h"

/* Copies a matrix element by element. */
void func_0020FF80(CMtxF dst, CMtxF src) {
    dst[0][0] = (f32) src[0][0];
    dst[0][1] = (f32) src[0][1];
    dst[0][2] = (f32) src[0][2];
    dst[0][3] = (f32) src[0][3];
    dst[1][0] = (f32) src[1][0];
    dst[1][1] = (f32) src[1][1];
    dst[1][2] = (f32) src[1][2];
    dst[1][3] = (f32) src[1][3];
    dst[2][0] = (f32) src[2][0];
    dst[2][1] = (f32) src[2][1];
    dst[2][2] = (f32) src[2][2];
    dst[2][3] = (f32) src[2][3];
    dst[3][0] = (f32) src[3][0];
    dst[3][1] = (f32) src[3][1];
    dst[3][2] = (f32) src[3][2];
    dst[3][3] = (f32) src[3][3];
}
