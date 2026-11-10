#include "common.h"
#include "structs/light.h"

s32 CGameObjectInstance__DrawModelImmediate(sptr, sptr, sptr);

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 unk40;
} Unk002605AC;

void EnemyAI_DrawFadeOut(s32 arg0, Unk002605AC *arg1, s32 arg2) {
    f32 var_f1;

    var_f1 = 255.0f - (arg1->unk40 * 8.5f);
    gBlendState = 1;
    if (var_f1 < 0.0f) {
        var_f1 = 0.0f;
    }
    gBlendState_m_Color[3] = var_f1;
    CGameObjectInstance__DrawModelImmediate(arg0, (sptr) arg1, arg2);
}
