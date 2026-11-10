#include "common.h"
#include "structs/game_object.h"

f32 func_00298470(f32);

/* Distance between two points (func_00298470 is the square root). */
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
f32 CVector3__Distance(CVector3 *arg0, CVector3 *arg1) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f2;

    temp_f0 = arg0->x - arg1->x;
    temp_f2 = arg0->y - arg1->y;
    temp_f12 = arg0->z - arg1->z;
    return func_00298470((temp_f0 * temp_f0) + (temp_f2 * temp_f2) + (temp_f12 * temp_f12));
}
#else
void CVector3__Distance(CVector3 *arg0, CVector3 *arg1) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f2;

    temp_f0 = arg0->x - arg1->x;
    temp_f2 = arg0->y - arg1->y;
    temp_f12 = arg0->z - arg1->z;
    func_00298470((temp_f0 * temp_f0) + (temp_f2 * temp_f2) + (temp_f12 * temp_f12));
}
#endif
