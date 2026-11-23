#include "common.h"
#include "structs/player.h"

void CPlayerPool__SetInvincibility(CPlayerPool *arg0, f32 arg1) {
    CPlayer *var_v0;

    var_v0 = arg0->m_ActiveList.m_pHead;
    if (var_v0 != NULL) {
        do {
            var_v0->m_InvincibilityTime = arg1;
            var_v0 = var_v0->m_pNext;
        } while (var_v0 != NULL);
    }
}
