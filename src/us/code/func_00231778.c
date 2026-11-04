#include "structs/particle.h"

extern u8 D_8011AAEF;

s32 func_00231778(CParticle *arg0) {
    s32 var_a1;

    var_a1 = 0;
    if (arg0->m_pEffect->m_dwFlags & 0x02000000) {
        if (arg0->m_dwFlags & 2) {
            var_a1 = 2;
        } else if (D_8011AAEF == 2) {
            var_a1 = 1;
        }
    }
    return var_a1;
}
