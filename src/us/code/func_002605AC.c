#include "common.h"

s32 CGameObjectInstance__DrawModelImmediate(s32, s32, s32);

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ f32 fadeScale;
} Unk800A7990;
extern Unk800A7990 D_800A7990;
extern s32 D_800AB8B0;
extern f32 D_800AB8C0;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 unk40;
} Unk002605AC;

void EnemyAI_DrawFadeOut(s32 arg0, Unk002605AC *arg1, s32 arg2) {
    f32 var_f1;

    var_f1 = 255.0f - (arg1->unk40 * D_800A7990.fadeScale);
    D_800AB8B0 = 1;
    if (var_f1 < 0.0f) {
        var_f1 = 0.0f;
    }
    D_800AB8C0 = var_f1;
    CGameObjectInstance__DrawModelImmediate(arg0, (s32) arg1, arg2);
}
