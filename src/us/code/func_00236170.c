#include "common.h"
#include "structs/game_object.h"
#include "structs/particle.h"
#include "structs/swoosh.h"

s32 CMtxF__FromMtxAffine(sptr, sptr);
s32 CMtxF__ClampEntries(sptr);
s32 Swoosh_NewNode(sptr, sptr, sptr);

s32 CMtxF__ScaleAxes(s32 *, f32, f32, f32);    /* extern */

void Swoosh_AddParticleNode(CSwooshSystem *arg0, CParticle *arg1, sptr arg2) {
    u8 sp10[0x40];
    f32 temp_f0;
    f32 var_f0;
    Swoosh *var_s0;

    var_s0 = arg0->m_SwooshPool.used.head;
    if (var_s0 != NULL) {
        do {
            if (var_s0->owner == arg1) {
                temp_f0 = var_s0->time;
                if (temp_f0 > 0.0f) {
                    if (temp_f0 > 0.0f) {
                        CMtxF__FromMtxAffine((sptr) sp10, arg2);
                        var_f0 = 0.01f;
                        if (arg1->m_pEffect->m_pModel != 0) {
                            var_f0 = 0.29257143f;
                        }
                        CMtxF__ScaleAxes(sp10, var_f0, var_f0, var_f0);
                        CMtxF__ClampEntries((sptr) sp10);
                        Swoosh_NewNode((sptr) arg0, (sptr) var_s0, (sptr) sp10);
                    }
                }
            }
            var_s0 = var_s0->next;
        } while (var_s0 != NULL);
    }
}
