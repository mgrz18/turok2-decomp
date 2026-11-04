#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

s32 func_00416AF8(s32);                             /* extern */

/* Counts the active players whose camera's menu pool is idle. */
s32 func_00254BE4(CPlayerList *arg0) {
    s32 var_s1;
    CPlayer *var_s0;

    var_s0 = arg0->m_ActiveList.m_pHead;
    var_s1 = 0;
    if (var_s0 != NULL) {
        do {
            if ((var_s0->m_bActive != 0) && (func_00416AF8((s32) var_s0->m_pCamera->m_MenuPool) == 0)) {
                var_s1 += 1;
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
    return var_s1;
}
