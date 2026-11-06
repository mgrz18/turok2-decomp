#include "common.h"
#include "structs/particle.h"

s32 CDLightSet__Free(s32 *, CDLight *);              /* extern */
s32 CParticle__BuildCameraMatrix();                            /* extern */
s32 Trail_DetachOwner(s32 *, void *);           /* extern */

extern s32 D_8010FF34;
extern s32 gEngineApp_m_SwooshSystem;

void CParticle__StopEffects(CParticle *arg0) {
    CDLight *temp_a1;

    if (arg0->m_ActiveSwooshes != 0) {
        CParticle__BuildCameraMatrix();
        Trail_DetachOwner(&gEngineApp_m_SwooshSystem, arg0);
    }
    temp_a1 = arg0->m_pDLight;
    if (temp_a1 != 0) {
        CDLightSet__Free(&D_8010FF34, temp_a1);
        arg0->m_pDLight = NULL;
    }
}
