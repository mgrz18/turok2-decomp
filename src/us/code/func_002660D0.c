#include "common.h"
#include "structs/game_object.h"

s32 Action_Draw(s32, s32, s32);
extern CAIModeInfo D_800B2410[];
extern void Action_Step(void);

/* AI init: mode table, step/draw callbacks, cleared mode scratch. */
void Action_Setup(CGameObjectInstance *arg0, CAI *arg1) {
    arg1->m_pModeTable = D_800B2410;
    arg1->m_pfStep = Action_Step;
    arg1->m_pfDraw = Action_Draw;
    arg1->m_VarBuffer[0] = 0;
    arg1->m_VarBuffer[1] = 0;
    arg1->m_VarBuffer[2] = 0;
    if (arg0->header.m_pIntelligence->m_dwTypeFlags & 1) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x10000);
        return;
    }
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & 0xFFFEFFFF);
}
