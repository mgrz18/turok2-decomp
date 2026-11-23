#include "common.h"
#include "globals/swoosh.h"
#include "structs/particle.h"
#include "globals/scene.h"

s32 CDLightSet__Free(s32 *, CDLight *);              /* extern */
s32 CParticle__BuildCameraMatrix();                            /* extern */
s32 Trail_DetachOwner(s32 *, void *);           /* extern */


/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CParticle__BuildCameraMatrix receives CParticle__StopEffects's own argument
 * registers (pass-through); the host passes them. */
void CParticle__StopEffects(CParticle *arg0) {
    CDLight *temp_a1;

    if (arg0->m_ActiveSwooshes != 0) {
        CParticle__BuildCameraMatrix(arg0);
        Trail_DetachOwner(&gEngineApp_m_SwooshSystem, arg0);
    }
    temp_a1 = arg0->m_pDLight;
    if (temp_a1 != 0) {
        CDLightSet__Free((s32 *) &gScene.m_DLightSet, temp_a1);
        arg0->m_pDLight = NULL;
    }
}
