#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern f32 D_800C0548;
extern s32 D_8012FA04;

void func_00407858(CGameObjectInstance *obj, CAI *ai) {
    f32 *animSpeed;

    if (ai->m_AnimRepeatedCount != 0) {
        /* through a pointer: a plain struct store gets scheduled later */
        animSpeed = &obj->m_AI.m_AnimSpeed;
        *animSpeed = (f32) D_800C0548;
        D_8012FA04 = -1;
        func_00243414(obj, ai, 0x13);
    }
}
