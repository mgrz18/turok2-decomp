#include "common.h"
#include "structs/game_object.h"

extern s32 D_800F1850;

void func_002317C0(CGameObjectInstance *arg0) {
    arg0->header.m_pIntelligence = (struct CIntelligence *) &D_800F1850;
    arg0->header.m_Type = 2;
    arg0->m_qGround.x = 0.0f;
    arg0->m_AI.m_pTargetOverride = 0;
    arg0->m_AI.m_pSightTarget = 0;
}
