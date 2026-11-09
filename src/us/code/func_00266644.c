#include "common.h"
#include "structs/game_object.h"
#include "structs/light.h"

s32 CGameObjectInstance__DrawModelImmediate(s32, sptr, s32);



void Action_DrawFade(s32 arg0, CAI *arg1, s32 arg2) {
    f32 var_f1;

    var_f1 = 255.0f - (arg1->m_ModeTime * 8.5f);
    gBlendState = 1;
    if (var_f1 < 0.0f) {
        var_f1 = 0.0f;
    }
    gBlendState_m_Color[3] = var_f1;
    CGameObjectInstance__DrawModelImmediate(arg0, (sptr) arg1, arg2);
}
