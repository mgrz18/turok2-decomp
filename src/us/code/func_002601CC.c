#include "common.h"
#include "structs/game_object.h"

f32 func_002119FC(f32, f32);                        /* extern */
s32 func_00243414(void *, void *, s32);     /* extern */
extern f32 D_800A797C;
extern f32 D_800A7980;
extern f32 D_800B6D28;

/* m_VarBuffer[6] holds a float timer here. */
s32 func_002601CC(CGameObjectInstance *arg0, CAI *arg1) {
    f32 temp_f0;
    s8 temp_v1;

    if (arg0->header.m_pIntelligence->m_Enemy.m_dwFlags & 0x40) {
        temp_f0 = *(f32 *) &arg1->m_VarBuffer[6] - D_800B6D28;
        *(f32 *) &arg1->m_VarBuffer[6] = temp_f0;
        if (temp_f0 < 0.0f) {
            *(f32 *) &arg1->m_VarBuffer[6] = 0.0f;
        }
        if (arg1->m_Health != 0) {
            if (*(f32 *) &arg1->m_VarBuffer[6] <= 0.0f) {
                temp_v1 = arg1->m_Mode;
                if ((temp_v1 == 4) || (temp_v1 == 0xE) || (temp_v1 == 0xF) || (temp_v1 == 0x10) || (temp_v1 == 0x11) || (temp_v1 == 0x12) || (temp_v1 == 0xA)) {
                    *(f32 *) &arg1->m_VarBuffer[6] = func_002119FC(D_800A797C, D_800A7980);
                    func_00243414(arg0, arg1, 0x17);
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
