#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

extern f32 D_800C0544;
extern s32 D_8012FA04;

void func_00407800(CGameObjectInstance *obj, CAI *ai) {
    f32 *animSpeed = &obj->m_AI.m_AnimSpeed;

    if (ai->m_AnimRepeatedCount != 0) {
        *animSpeed = D_800C0544;
        D_8012FA04 = -1;
        func_00243414((s32) obj, (s32) ai, 0x13);
    }
}
