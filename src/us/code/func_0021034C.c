#include "common.h"

/* Scales the matrix's three axis rows by arg1, arg2, arg3. */
void CMtxF__ScaleAxes(f32 (*arg0)[4], f32 arg1, f32 arg2, f32 arg3) {
    arg0[0][0] = (arg0[0][0] * arg1);
    arg0[0][1] = (arg0[0][1] * arg1);
    arg0[0][2] = (arg0[0][2] * arg1);
    arg0[1][0] = (arg0[1][0] * arg2);
    arg0[1][1] = (arg0[1][1] * arg2);
    arg0[1][2] = (arg0[1][2] * arg2);
    arg0[2][0] = (arg0[2][0] * arg3);
    arg0[2][1] = (arg0[2][1] * arg3);
    arg0[2][2] = (arg0[2][2] * arg3);
}
