#include "structs/particle.h"
#include "structs/swoosh.h"

s32 CMtxF__FromMtxAffine(sptr, sptr);
s32 CMtxF__ClampEntries(sptr);
sptr Swoosh_NewNode(sptr, sptr, sptr);
s32 CMtxF__ScaleAxes(u8 *, f32, f32, f32);    /* extern */

/* Adds a point at matrix arg3 to swoosh arg1 when arg2 is its owner particle and it is still live. */
void Swoosh_AddNodeFromMtx(s32 arg0, Swoosh *arg1, CParticle *arg2, s32 arg3) {
    u8 sp10[0x40];
    f32 var_f0;
    CParticle *temp_s0;

    temp_s0 = arg1->owner;
    if ((temp_s0 == arg2) && (arg1->time > 0.0f)) {
        CMtxF__FromMtxAffine((sptr) sp10, arg3);
        var_f0 = 0.01f;
        if (temp_s0->m_pEffect->m_pModel != NULL) {
            var_f0 = 0.29257143f;
        }
        CMtxF__ScaleAxes(sp10, var_f0, var_f0, var_f0);
        CMtxF__ClampEntries((sptr) sp10);
        Swoosh_NewNode(arg0, (sptr) arg1, (sptr) sp10);
    }
}
