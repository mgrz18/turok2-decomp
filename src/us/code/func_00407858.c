#include "common.h"
#include "structs/game_object.h"
#include "globals/boss.h"

s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern f32 D_800C0548;

void Primagen_UpdateMode22(CGameObjectInstance *obj, CAI *ai) {
    f32 *animSpeed;

    if (ai->m_AnimRepeatedCount != 0) {
        /* through a pointer: a plain struct store gets scheduled later */
        animSpeed = &obj->m_AI.m_AnimSpeed;
        *animSpeed = (f32) D_800C0548;
        gLevelScript.script.current = -1;
        CAI__SetMode(obj, ai, 0x13);
    }
}
