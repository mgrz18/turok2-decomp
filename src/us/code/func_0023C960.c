#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

void CVector3__Add(CVector3 *, CVector3 *, CVector3 *);

void Quad_Translate(CVector3 *arg0, CVector3 *arg1) {
    CVector3 *temp_a0;
    CVector3 *temp_a0_2;
    CVector3 *temp_s0;

    CVector3__Add(arg0, arg0, arg1);
    temp_a0 = ((__typeof__(arg0))((s8 *)arg0 + 0xC));
    CVector3__Add(temp_a0, temp_a0, arg1);
    temp_a0_2 = ((__typeof__(arg0))((s8 *)arg0 + 0x18));
    CVector3__Add(temp_a0_2, temp_a0_2, arg1);
    temp_s0 = ((__typeof__(arg0))((s8 *)arg0 + 0x24));
    CVector3__Add(temp_s0, temp_s0, arg1);
}
