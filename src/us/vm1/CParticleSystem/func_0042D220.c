/* WRONG CUT: REAL CParticleSystem__Construct size 0x1E8 (splat cut it into CParticleSystem__Construct + func_0042D330). */
#include "common.h"
#include "structs/particle.h"
#include "globals/scene.h"
#include "globals/particle.h"
#include "protos/list.h"
#include "structs/gbi.h"

/* One of the four default light directions. */

extern Vtx_t gParticleSpriteVtx[4];

extern void CIntelligence__Init(void *def);
extern void CMtxF__SetRotateX(void *, f32);

void CParticleSystem__Construct(CParticleSystem *sys, sptr arg1) { /* the particle data pointer-wide */
    s32 i;
    s32 j;
    Vtx_t *light;

    for (i = 0; i < 128; i++) {
        sys->m_ParticleVars[i].header.m_pIntelligence = (CIntelligence *)&gParticleIntelligence;
        sys->m_ParticleVars[i].header.m_Type = 2;
        sys->m_ParticleVars[i].m_dwFlags = 0;
        sys->m_ParticleVars[i].m_pLoopSound = NULL;
        sys->m_ParticleVars[i].m_pActiveList = NULL;
    }
    CList__Construct(&sys->m_FreeList, T2_OFFSETOF(CParticle, m_pLast), T2_OFFSETOF(CParticle, m_pNext));
    for (i = 0; i < 128; i++) {
        CList__AddTail(&sys->m_FreeList, &sys->m_ParticleVars[i]);
    }
    CList__Construct(&sys->m_DrawList, T2_OFFSETOF(CParticle, m_pDrawLast), T2_OFFSETOF(CParticle, m_pDrawNext));
    for (i = 0; i < 3; i++) {
        CList__Construct(&sys->m_ActiveList[i], T2_OFFSETOF(CParticle, m_pLast), T2_OFFSETOF(CParticle, m_pNext));
    }
    CIntelligence__Init(&gParticleIntelligence);
    for (j = 0; j < 4; j++) {
        light = &gParticleSpriteVtx[j];
        switch (j) {
            case 0:
                light->ob[0] = -1;
                light->ob[1] = 1;
                light->tc[0] = -0x8000;
                light->tc[1] = -0x8000;
                break;
            case 1:
                light->ob[0] = 1;
                light->ob[1] = 1;
                light->tc[0] = 0;
                light->tc[1] = -0x8000;
                break;
            case 2:
                light->ob[0] = 1;
                light->ob[1] = -1;
                light->tc[0] = 0;
                light->tc[1] = 0;
                break;
            case 3:
                light->ob[0] = -1;
                light->ob[1] = -1;
                light->tc[0] = -0x8000;
                light->tc[1] = 0;
                break;
        }
        light->ob[2] = 0;
        light->flag = 0;
        light->cn[0] = 0xFF;
        light->cn[1] = 0xFF;
        light->cn[2] = 0xFF;
        light->cn[3] = 0xFF;
    }
    CMtxF__SetRotateX(sParticleRotXMtx, 1.5707964f);
    sys->m_pParticleData = (void *)arg1;
}
