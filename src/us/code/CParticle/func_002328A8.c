#include "common.h"
#include "structs/game_object.h"
#include "structs/particle.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/scene.h"

void Render_QueueModelByResource(void *model, void *mtx, void *mtx2, s32 a, s32 b);

void CParticle__DrawModel(CParticle *arg0) {
    CScene__GetLightsAt(&gScene, &arg0->header.m_vPos, &arg0->m_Lights[gEvenOdd]);
    Render_QueueModelByResource(arg0->m_pEffect->m_pModel, arg0->m_mtxDraw[gEvenOdd], &arg0->m_Lights[gEvenOdd], 0, -1);
}
