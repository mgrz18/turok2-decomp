#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"

f32 Random_Blend(f32, f32);                        /* extern */
s32 CAI__SetMode(void *, void *, s32);     /* extern */
extern f32 D_800A797C;
extern f32 D_800A7980;

/* m_VarBuffer[6] holds a float timer here. */
s32 EnemyAI_TryTeleportOut(CGameObjectInstance *arg0, CAI *arg1) {
    f32 temp_f0;
    s8 temp_v1;

    if (arg0->header.m_pIntelligence->m_Enemy.m_dwFlags & 0x40) {
        temp_f0 = *(f32 *) &arg1->m_VarBuffer[6] - gFrameIncrement;
        *(f32 *) &arg1->m_VarBuffer[6] = temp_f0;
        if (temp_f0 < 0.0f) {
            *(f32 *) &arg1->m_VarBuffer[6] = 0.0f;
        }
        if (arg1->m_Health != 0) {
            if (*(f32 *) &arg1->m_VarBuffer[6] <= 0.0f) {
                temp_v1 = arg1->m_Mode;
                if ((temp_v1 == 4) || (temp_v1 == 0xE) || (temp_v1 == 0xF) || (temp_v1 == 0x10) || (temp_v1 == 0x11) || (temp_v1 == 0x12) || (temp_v1 == 0xA)) {
                    *(f32 *) &arg1->m_VarBuffer[6] = Random_Blend(D_800A797C, D_800A7980);
                    CAI__SetMode(arg0, arg1, 0x17);
                    return 1;
                }
                /* Duplicate return node #14. Try simplifying control flow for better match */
                return 0;
            }
            goto block_13;
        }
        /* Duplicate return node #14. Try simplifying control flow for better match */
        return 0;
    }
block_13:
    return 0;
}
