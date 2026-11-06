#include "common.h"
#include "structs/game_object.h"

s32 CGameObjectInstance__DrawModelImmediate(s32, s32, s32);

extern f32 D_800A7CAC;

extern s32 gBlendState;
extern f32 gBlendFade;

void Action_DrawFade(s32 arg0, CAI *arg1, s32 arg2) {
    f32 var_f1;

    var_f1 = D_800A7CAC - (arg1->m_ModeTime * 8.5f);
    gBlendState = 1;
    if (var_f1 < 0.0f) {
        var_f1 = 0.0f;
    }
    gBlendFade = var_f1;
    CGameObjectInstance__DrawModelImmediate(arg0, (s32) arg1, arg2);
}
