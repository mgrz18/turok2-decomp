#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern f32 Random_Blend(f32 lo, f32 hi);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

void Primagen_UpdateMode5(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        D_8012F9DC.mode = 5;
        D_8012F9DC.timer = Random_Blend(0.5f, 4.0f) * 15.0f;
        CAI__SetMode(obj, ai, 1);
    }
    if (D_8012F9DC.mode == 0) {
        CAI__SetMode(obj, ai, 6);
    }
}
