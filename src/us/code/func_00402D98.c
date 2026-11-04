#include "common.h"
#include "structs/game_object.h"

f32 func_0021170C(f32, f32);
s32 func_00243414(s32, s32, s32);

extern f32 D_800C0354;

void func_00402D98(s32 arg0, CAI *arg1) {
    f32 temp_f20;

    temp_f20 = D_800C0354;
    arg1->m_vDesiredPos.x = func_0021170C(arg1->m_vDesiredPos.x, temp_f20);
    arg1->m_vDesiredPos.z = func_0021170C(arg1->m_vDesiredPos.z, temp_f20);
    if (arg1->m_AnimRepeatedCount != 0) {
        func_00243414(arg0, (s32) arg1, 5);
    }
}
