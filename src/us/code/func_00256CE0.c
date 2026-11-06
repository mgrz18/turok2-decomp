#include "common.h"
#include "structs/player.h"

f32 CVector3__Distance(s32, void *);                 /* extern */

f32 CPlayerPool__SumViewWeights(CPlayerPool *arg0, s32 arg1) {
    f32 var_f20;
    CPlayer *var_s0;

    var_s0 = arg0->m_ActiveList.m_pHead;
    var_f20 = 0.0f;
    if (var_s0 != NULL) {
        do {
            if (var_s0->m_pCamera != 0) {
                var_f20 += CVector3__Distance(arg1, &var_s0->m_CharacterActor.header.m_vPos);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
    return var_f20;
}
