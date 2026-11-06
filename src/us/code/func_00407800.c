#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);

extern f32 D_800C0544;
extern s32 gLevelScript_script_current;

void Primagen_UpdateMode21(CGameObjectInstance *obj, CAI *ai) {
    f32 *animSpeed = &obj->m_AI.m_AnimSpeed;

    if (ai->m_AnimRepeatedCount != 0) {
        *animSpeed = D_800C0544;
        gLevelScript_script_current = -1;
        CAI__SetMode((s32) obj, (s32) ai, 0x13);
    }
}
