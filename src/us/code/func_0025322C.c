#include "common.h"
#include "structs/player.h"

s32 CPlayer__DrawHud(void *, sptr);                 /* extern */
s32 CCamera__LoadOnScreenProjection(sptr);                         /* extern */

void CPlayerPool__DrawHuds(CPlayerPool *arg0, sptr arg1) {
    CPlayer *var_s0;

    CCamera__LoadOnScreenProjection(arg1);
    var_s0 = arg0->m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if ((sptr) var_s0->m_pCamera == arg1) {
                CPlayer__DrawHud(var_s0, arg1);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
}
