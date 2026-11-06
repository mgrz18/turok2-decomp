#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

void CVector3__Subtract(CVector3 *arg0, CVector3 *arg1, CVector3 *arg2);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);

void Quad_ComputeNormal(CVector3 *arg0) {
    CVector3 sp10;
    CVector3 sp20;
    CVector3 *temp_s1;

    temp_s1 = ((__typeof__(arg0))((s8 *)arg0 + 0xC));
    CVector3__Subtract(&sp10, temp_s1, arg0);
    CVector3__Subtract(&sp20, ((__typeof__(arg0))((s8 *)arg0 + 0x18)), temp_s1);
    CVector3__Cross(((__typeof__(arg0))((s8 *)arg0 + 0x30)), &sp10, &sp20);
}
