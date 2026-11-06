#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);
s32 MantidQueen_Hurt(s32, s32, s32);
s32 MantidQueen_Step(s32, s32);
s32 MantidQueen_Draw(s32, s32, s32);

extern s32 D_00430CA4;
extern s32 D_80110048;
extern s32 EnemyAI_HeadNodeCallback;

void MantidQueen_InitAI(s32 arg0, CAI *arg1) {
    D_80110048 = 0x1D4D;
    arg1->m_pModeTable = (CAIModeInfo *) &D_00430CA4;
    arg1->m_pfStep = MantidQueen_Step;
    arg1->m_pfHurt = MantidQueen_Hurt;
    arg1->m_pfEffects = &EnemyAI_HeadNodeCallback;
    arg1->m_pfDraw = MantidQueen_Draw;
    arg1->m_VisBits = -1;
    CAI__SetMode(arg0, (s32) arg1, 0);
}
