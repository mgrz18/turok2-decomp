#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

void func_0025E658(CGameObjectInstance *arg0, s32 arg1) {
    CIntelligence *temp_a2;

    temp_a2 = arg0->header.m_pIntelligence;
    if ((temp_a2->m_Type == 1) && !(temp_a2->m_dwTypeFlags & 0x800)) {
        func_00243414((s32) arg0, arg1, 2);
    }
}
