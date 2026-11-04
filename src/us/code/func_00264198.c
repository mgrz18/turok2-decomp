#include "common.h"
#include "structs/game_object.h"

void func_0026426C(s32, s32);
s32 func_00264378(s32, s32);

extern s32 D_800B20B0;

void func_00264198(CGameObjectInstance *arg0, CAI *arg1) {
    CTurretIntelligence *temp_v0;
    CIntelligence *temp_v1;

    temp_v1 = arg0->header.m_pIntelligence;
    arg1->m_pModeTable = (struct CAIModeInfo *) &D_800B20B0;
    arg1->m_pfStep = func_00264378;
    arg1->m_pfCalculateOrientationMatrix = func_0026426C;
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
