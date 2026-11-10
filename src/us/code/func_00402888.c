#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "globals/boss_ai.h"
#include "protos/ai.h"

s32 MantidQueen_Hurt(s32, s32, s32);
s32 MantidQueen_Step(s32, sptr);
s32 MantidQueen_Draw(sptr, sptr, sptr);

extern s32 EnemyAI_HeadNodeCallback;

void MantidQueen_InitAI(sptr arg0, CAI *arg1) {
    gScene_m_LevelID = 0x1D4D;
    arg1->m_pModeTable = (CAIModeInfo *) sMantidQueenModeTable;
    arg1->m_pfStep = MantidQueen_Step;
    arg1->m_pfHurt = MantidQueen_Hurt;
    arg1->m_pfEffects = &EnemyAI_HeadNodeCallback;
    arg1->m_pfDraw = MantidQueen_Draw;
    arg1->m_VisBits = -1;
    CAI__SetMode(arg0, arg1, 0);
}
