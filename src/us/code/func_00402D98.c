#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

f32 Math_DecayToZero(f32, f32);

void MantidQueen_UpdateMode4(s32 arg0, CAI *arg1) {
    f32 temp_f20;

    temp_f20 = 51.199997f;
    arg1->m_vDesiredPos.x = Math_DecayToZero(arg1->m_vDesiredPos.x, temp_f20);
    arg1->m_vDesiredPos.z = Math_DecayToZero(arg1->m_vDesiredPos.z, temp_f20);
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 5);
    }
}
