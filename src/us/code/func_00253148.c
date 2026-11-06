#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

s32 Render_FlushBatches(void);
s32 CPlayer__DrawCharacter(s32, s32);
s32 Cinema_FollowsPlayer(void);

void Render_ResetBatches();                               /* extern */

void CPlayerPool__DrawCharacters(CPlayerList *arg0, CCamera *arg1) {
    CPlayer *var_s0;

    Render_ResetBatches();
    var_s0 = arg0->m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if ((var_s0->m_pCamera != arg1) || (arg1->m_ViewParams.m_nType == 1) || (Cinema_FollowsPlayer() != 0)) {
                CPlayer__DrawCharacter((s32) var_s0, (s32) arg1);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
    Render_FlushBatches();
}
