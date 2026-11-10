#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/model.h"
#include "globals/boss.h"
#include "globals/engine.h"
#include "structs/light.h"
#include "protos/game.h"

s32 CGameObjectInstance__DrawModel(sptr, sptr, sptr);
s32 ScreenTint_UpdateMode(void);
s32 ScriptAnimator_Apply(void *);

void BlindOne_Group3Draw(CGameObjectInstance *arg0, CAI *arg1, CDrawArgs *arg2)
{
  BlindOneState *state;
  state = &gBlindOne;
  CGameObjectInstance__DrawModel((sptr) arg0, (sptr) arg1, (sptr) arg2);
  if (arg1->m_Health != 0)
  {
    ScriptAnimator_Apply(&state->faders[arg1->m_BlindOne.m_nFader]);
    ScreenTint_UpdateMode();
    Render_DrawModel(arg2->m_hModel, arg0->m_pmtDrawMtxs, 1, &arg0->m_Lights[gEvenOdd], 0, 1, arg0->m_AI.m_nTexture);
  }
  gBlendState = 0;
}
