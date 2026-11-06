#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"


void func_002317C0(CGameObjectInstance *arg0) {
    arg0->header.m_pIntelligence = (struct CIntelligence *) &gParticleIntelligence;
    arg0->header.m_Type = 2;
    arg0->m_qGround.x = 0.0f;
    arg0->m_AI.m_pTargetOverride = 0;
    arg0->m_AI.m_pSightTarget = 0;
}
