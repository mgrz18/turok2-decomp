#include "common.h"

/* Translates the matrix by (arg1, arg2, arg3) in its own axes. */
void func_00210284(f32 (*arg0)[4], f32 arg1, f32 arg2, f32 arg3) {
    arg0[3][0] = (arg0[3][0] + ((arg1 * arg0[0][0]) + (arg2 * arg0[1][0]) + (arg3 * arg0[2][0])));
    arg0[3][1] = (arg0[3][1] + ((arg1 * arg0[0][1]) + (arg2 * arg0[1][1]) + (arg3 * arg0[2][1])));
    arg0[3][2] = (arg0[3][2] + ((arg1 * arg0[0][2]) + (arg2 * arg0[1][2]) + (arg3 * arg0[2][2])));
}
