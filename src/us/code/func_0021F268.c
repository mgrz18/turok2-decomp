#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

f32 func_00298470(f32);

void func_0021F268(CGameObjectInstance *arg0) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f1;

    temp_f12 = arg0->m_vScale.x;
    temp_f1 = arg0->m_vScale.y;
    temp_f0 = arg0->m_vScale.z;
    func_00298470(((temp_f12 * temp_f12) + (temp_f1 * temp_f1) + (temp_f0 * temp_f0)) * 0.33333334f);
}
