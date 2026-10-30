#include "common.h"
#include "structs/game_object.h"

extern f32 func_002119FC(f32 lo, f32 hi);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

/* Boss module state at 0x8012F9DC. */
typedef struct {
    /* 0x0 */ s32 mode;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ f32 timer;
} BossState;
extern BossState D_8012F9DC;

void func_00407374(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        D_8012F9DC.mode = 5;
        D_8012F9DC.timer = func_002119FC(0.5f, 4.0f) * 15.0f;
        func_00243414(obj, ai, 1);
    }
    if (D_8012F9DC.mode == 0) {
        func_00243414(obj, ai, 6);
    }
}
