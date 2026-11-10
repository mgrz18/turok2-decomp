#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

f32 func_00298470(f32);

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
f32 CGameObjectInstance__GetAverageScale(CGameObjectInstance *arg0) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f1;

    temp_f12 = arg0->m_vScale.x;
    temp_f1 = arg0->m_vScale.y;
    temp_f0 = arg0->m_vScale.z;
    return func_00298470(((temp_f12 * temp_f12) + (temp_f1 * temp_f1) + (temp_f0 * temp_f0)) * 0.33333334f);
}
#else
void CGameObjectInstance__GetAverageScale(CGameObjectInstance *arg0) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f1;

    temp_f12 = arg0->m_vScale.x;
    temp_f1 = arg0->m_vScale.y;
    temp_f0 = arg0->m_vScale.z;
    func_00298470(((temp_f12 * temp_f12) + (temp_f1 * temp_f1) + (temp_f0 * temp_f0)) * 0.33333334f);
}
#endif
