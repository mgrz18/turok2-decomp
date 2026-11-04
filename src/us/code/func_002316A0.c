#include "structs/particle.h"

extern u8 D_8011AAEF;

void func_002316A0(CParticle *arg0, s32 *arg1) {
    s32 var_a2;

    var_a2 = 0;
    if (arg0->m_pEffect->m_dwFlags & 0x02000000) {
        if (arg0->m_dwFlags & 2) {
            var_a2 = 2;
        } else if (D_8011AAEF == 2) {
            var_a2 = 1;
        }
    }
    *arg1 |= var_a2 << 0x1E;
}
