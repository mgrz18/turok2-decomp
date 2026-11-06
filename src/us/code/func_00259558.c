#include "common.h"
#include "structs/game_object.h"

s32 WeaponAnim_Alternate4F1A(s32 arg0, CAI *ai) {
    f32 temp_f0;
    s32 var_v0;

    var_v0 = 0x4F1A;
    temp_f0 = -*(f32 *) &ai->m_VarBuffer[2];
    *(f32 *) &ai->m_VarBuffer[2] = temp_f0;
    if (!(temp_f0 > 0.0f)) {
        var_v0 = 0x4F24;
    }
    return var_v0;
}
