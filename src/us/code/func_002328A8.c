#include "common.h"
#include "structs/game_object.h"
#include "structs/particle.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/scene.h"

s32 func_0020B9F4(s32, void *, void *, s32, s32); /* extern */

void CParticle__DrawModel(CParticle *arg0) {
    CScene__GetLightsAt(&gScene, &arg0->header.m_vPos, &arg0->m_Lights[gEvenOdd]);
    func_0020B9F4(arg0->m_pEffect->m_pModel, arg0->m_mtxDraw[gEvenOdd], &arg0->m_Lights[gEvenOdd], 0, -1);
}
