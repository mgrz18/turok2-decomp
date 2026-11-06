/* Second VM module copy of CParticleSystem__Construct (same source, linked 0xE0 lower). */
#include "common.h"
#include "structs/particle.h"
#include "globals/scene.h"
#include "globals/particle.h"

/* One of the four default light directions. */
typedef struct DirLight {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 w;
    /* 0x08 */ s16 angle0;
    /* 0x0A */ s16 angle1;
    /* 0x0C */ u8 color[4];
} DirLight;

extern DirLight gParticleSpriteVtx[4];

extern void CList__Construct(CList *list, s32 prevOffset, s32 nextOffset);
extern s32 CList__AddTail(CList *list, void *node);
extern void func_0027BD10(void *def);
extern void CMtxF__SetRotateX(void *, f32);

void CParticleSystem__Construct_2(CParticleSystem *sys, s32 arg1) {
    s32 i;
    s32 j;
    DirLight *light;

    for (i = 0; i < 128; i++) {
        sys->m_ParticleVars[i].header.m_pIntelligence = (CIntelligence *)&gParticleIntelligence;
        sys->m_ParticleVars[i].header.m_Type = 2;
        sys->m_ParticleVars[i].m_dwFlags = 0;
        sys->m_ParticleVars[i].unk_1B8 = 0;
        sys->m_ParticleVars[i].m_pActiveList = NULL;
    }
    CList__Construct(&sys->m_FreeList, 0x1C4, 0x1C8);
    for (i = 0; i < 128; i++) {
        CList__AddTail(&sys->m_FreeList, &sys->m_ParticleVars[i]);
    }
    CList__Construct(&sys->m_DrawList, 0x1CC, 0x1D0);
    for (i = 0; i < 3; i++) {
        CList__Construct(&sys->m_ActiveList[i], 0x1C4, 0x1C8);
    }
    func_0027BD10(&gParticleIntelligence);
    for (j = 0; j < 4; j++) {
        light = &gParticleSpriteVtx[j];
        switch (j) {
            case 0:
                light->x = -1;
                light->y = 1;
                light->angle0 = -0x8000;
                light->angle1 = -0x8000;
                break;
            case 1:
                light->x = 1;
                light->y = 1;
                light->angle0 = 0;
                light->angle1 = -0x8000;
                break;
            case 2:
                light->x = 1;
                light->y = -1;
                light->angle0 = 0;
                light->angle1 = 0;
                break;
            case 3:
                light->x = -1;
                light->y = -1;
                light->angle0 = -0x8000;
                light->angle1 = 0;
                break;
        }
        light->z = 0;
        light->w = 0;
        light->color[0] = 0xFF;
        light->color[1] = 0xFF;
        light->color[2] = 0xFF;
        light->color[3] = 0xFF;
    }
    CMtxF__SetRotateX(sParticleRotXMtx, 1.5707964f);
    sys->m_pParticleData = (void *)arg1;
}
