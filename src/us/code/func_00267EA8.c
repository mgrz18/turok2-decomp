#include "common.h"
#include "structs/game_object.h"

extern u8 D_8011AAF1;

s32 func_00267EA8(CGameObjectInstance *arg0) {
    CIntelligence *temp_a0;

    temp_a0 = arg0->header.m_pIntelligence;
    if (temp_a0->m_Type == 0xB) {
        if (D_8011AAF1 != 0) {
            return temp_a0->m_Player.m_dwFlags & 1;
        }
        return 1;
    }
    return 0;
}
