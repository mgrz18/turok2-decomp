#include "common.h"
#include "m2c_macros.h"

s32 CEngineApp__UpdateCinemaMode(s32, s32, s32);

extern s32 gFinalEnding[];
extern s32 gMergedController_buttonsSingle[];
extern s32 gCreditsDone[]; /* local type: globals/menu.h declares it; this view (its type here) is what keeps the ROM's code */
extern f32 gCreditsTimeLeft[]; /* local type: globals/menu.h declares it; this view (its type here) is what keeps the ROM's code */

void EngineMode15_Ending_Retrace(s32 arg0)
{
  int new_var;
  new_var = 0;
  if (gFinalEnding[0] != 0)
  {
    CEngineApp__UpdateCinemaMode(arg0, 1, 1);
    return;
  }
  if ((gMergedController_buttonsSingle[0] & 0x1000) && (gCreditsDone[new_var] == 0))
  {
    gCreditsDone[new_var] = 1;
    gMergedController_buttonsSingle[new_var] = 0;
    gCreditsTimeLeft[new_var] = 15.0f;
  }
}
