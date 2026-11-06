#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

void CPlayer__DrawWeaponOverlay(void *, void *);                 /* extern */

void CPlayerPool__DrawWeaponOverlays(CPlayerPool *arg0, CCamera *arg1) {
    CPlayer *var_s0;

    var_s0 = arg0->m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if ((var_s0->m_pCamera == arg1) && (arg1->m_ViewParams.m_nType == 0)) {
                CPlayer__DrawWeaponOverlay(var_s0, arg1);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
}
