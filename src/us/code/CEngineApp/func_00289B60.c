#include "common.h"
#include "m2c_macros.h"
#include "globals/engine.h"

s32 CEngineApp__UpdateCinemaMode(sptr, s32, s32);

/* one-element array views of scalars (globals/controller.h, globals/menu.h), indexed by a zero variable for the match */
extern s32 gButtonsSingleView[] __asm__("gMergedController_buttonsSingle");
extern s32 gCreditsDoneView[] __asm__("gCreditsDone");
extern f32 gCreditsTimeLeftView[] __asm__("gCreditsTimeLeft");

void EngineMode15_Ending_Retrace(sptr arg0)
{
  int new_var;
  new_var = 0;
  if (gFinalEnding != 0)
  {
    CEngineApp__UpdateCinemaMode(arg0, 1, 1);
    return;
  }
  if ((gButtonsSingleView[0] & 0x1000) && (gCreditsDoneView[new_var] == 0))
  {
    gCreditsDoneView[new_var] = 1;
    gButtonsSingleView[new_var] = 0;
    gCreditsTimeLeftView[new_var] = 15.0f;
  }
}
