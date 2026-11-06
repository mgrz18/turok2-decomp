#include "common.h"
#include "m2c_macros.h"
#include "structs/vector.h"

void CVector3__Scale(CVector3 *arg0, CVector3 *arg1, f32 arg2) {
    arg0->x = arg1->x * arg2;
    arg0->y = arg1->y * arg2;
    arg0->z = arg1->z * arg2;
}
