#include "common.h"
#include "structs/game_object.h"

/* Copies the matrix's translation row to arg1. */
void func_002101A0(f32 (*arg0)[4], CVector3 *arg1) {
    arg1->x = arg0[3][0];
    arg1->y = arg0[3][1];
    arg1->z = arg0[3][2];
}
