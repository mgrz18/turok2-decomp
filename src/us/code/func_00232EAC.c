#include "common.h"
#include "structs/particle.h"

f32 func_00232EAC(CParticle *arg0) {
    f32 var_f0;

    var_f0 = 0.0f;
    if (arg0->m_pEffect->m_pModel == NULL) {
        var_f0 = arg0->m_vRot.x;
    }
    return var_f0;
}
