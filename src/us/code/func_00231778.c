#include "structs/particle.h"
#include "globals/game.h"


s32 func_00231778(CParticle *arg0) {
    s32 var_a1;

    var_a1 = 0;
    if (arg0->m_pEffect->m_dwFlags & 0x02000000) {
        if (arg0->m_dwFlags & 2) {
            var_a1 = 2;
        } else if (gEngineApp_m_Settings.unk17 == 2) {
            var_a1 = 1;
        }
    }
    return var_a1;
}
