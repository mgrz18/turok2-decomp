#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);
s32 func_00264C8C(void *, void *, s32);         /* extern */

extern CGameObjectInstance *D_800F1CE0;        /* the player */

void func_0025EF68(s32 arg0, CAI *arg1) {
    void *owner;

    if ((arg1->m_Health != 0) && (D_800F1CE0 != NULL) && (owner = arg1->m_pPathTarget, !(arg1->m_ModeFlags & 0x1000)) && (D_800F1CE0 == owner) && (*(s32 *) D_800F1CE0->header.m_pIntelligence == 5)) {
        arg1->m_VarBuffer[2] = (u32) D_800F1CE0;
        func_00264C8C(D_800F1CE0, &D_800F1CE0->m_AI, arg0);
        func_00243414(arg0, (s32) arg1, 0x15);
    }
}
