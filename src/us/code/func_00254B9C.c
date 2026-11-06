#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

/* Counts the players with no menu open on their camera and m_LastTagMode clear. */
s32 CPlayerPool__CountPlaying(CPlayerPool *arg0) {
    s32 var_a0;
    CCamera *temp_v0;
    CPlayer *var_v1;

    var_v1 = arg0->m_ActiveList.m_pHead;
    var_a0 = 0;
    if (var_v1 != NULL) {
        do {
            temp_v0 = var_v1->m_pCamera;
            if ((temp_v0 != NULL) && (temp_v0->m_Menus.count == 0) && (var_v1->m_pOptions->m_LastTagMode == 0)) {
                var_a0 += 1;
            }
            var_v1 = var_v1->m_pNext;
        } while (var_v1 != NULL);
    }
    return var_a0;
}
