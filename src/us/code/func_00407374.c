#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern f32 func_002119FC(f32 lo, f32 hi);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

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
