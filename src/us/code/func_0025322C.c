#include "common.h"
#include "structs/player.h"

s32 CPlayer__DrawHud(void *, s32);                 /* extern */
s32 CCamera__LoadOnScreenProjection(s32);                         /* extern */

void CPlayerPool__DrawHuds(CPlayerPool *arg0, s32 arg1) {
    CPlayer *var_s0;

    CCamera__LoadOnScreenProjection(arg1);
    var_s0 = arg0->m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if ((s32) var_s0->m_pCamera == arg1) {
                CPlayer__DrawHud(var_s0, arg1);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
}
