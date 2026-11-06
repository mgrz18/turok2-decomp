#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/model.h"

s32 Render_DrawModel(s32, s32, s32, s32, s32, s32, s32);
s32 CGameObjectInstance__DrawModel(s32, s32, s32);
s32 ScreenTint_UpdateMode(void);
s32 ScriptAnimator_Apply(void *);
extern s32 D_800AB8B0;
extern s32 D_800B6D1C;
extern BlindOneState D_80130200;

void BlindOne_Group3Draw(CGameObjectInstance *arg0, CAI *arg1, CDrawArgs *arg2)
{
  BlindOneState *state;
  state = &D_80130200;
  CGameObjectInstance__DrawModel((s32) arg0, (s32) arg1, (s32) arg2);
  if (arg1->m_Health != 0)
  {
    ScriptAnimator_Apply(&state->faders[arg1->m_BlindOne.m_nFader]);
    ScreenTint_UpdateMode();
    Render_DrawModel(arg2->m_hModel, (s32) arg0->m_pmtDrawMtxs, 1, (s32) arg0->m_Lights[D_800B6D1C], 0, 1, arg0->m_AI.m_nTexture);
  }
  D_800AB8B0 = 0;
}
