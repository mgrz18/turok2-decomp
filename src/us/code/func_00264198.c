#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

void Platform_CalcOrientation(s32, s32);
s32 Platform_Step(sptr, sptr);

void Platform_Setup(CGameObjectInstance *arg0, CAI *arg1) {
    CTurretIntelligence *temp_v0;
    CIntelligence *temp_v1;

    temp_v1 = arg0->header.m_pIntelligence;
    arg1->m_pModeTable = gPlatformModeTable;
    arg1->m_pfStep = Platform_Step;
    arg1->m_pfCalculateOrientationMatrix = Platform_CalcOrientation;
    arg1->m_VarBuffer[4] = 0;
    arg1->m_VarBuffer[5] = 0;
    temp_v0 = &temp_v1->m_Turret;
    arg1->m_ConditionTimer = temp_v0->m_RotSpeed;
    arg1->m_VarBuffer[0] = 0;
    arg1->m_VarBuffer[1] = 0;
    arg1->m_VarBuffer[2] = 0;
    arg1->m_VarBuffer[3] = 0;
    *(f32 *)&arg1->m_VarBuffer[6] = temp_v0->m_RotSpeed * 6.283186f;
    if (temp_v1->m_dwTypeFlags & 4) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x2000);
    }
}
