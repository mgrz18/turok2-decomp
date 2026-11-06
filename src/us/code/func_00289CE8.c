#include "common.h"
#include "m2c_macros.h"
#include "globals/menu.h"

s32 CEngineApp__UpdateCinemaMode(s32, s32, s32);

extern f32 D_800A9E20;
extern s32 gFinalEnding;
extern s32 gMergedController_buttonsSingle;

void EngineMode16_Ending_Retrace(s32 arg0) {
    if (gFinalEnding != 0) {
        CEngineApp__UpdateCinemaMode(arg0, 1, 1);
        return;
    }
    if ((gMergedController_buttonsSingle & 0x1000) && (gCreditsDone == 0)) {
        gCreditsDone = 1;
        gMergedController_buttonsSingle = 0;
        gCreditsTimeLeft = D_800A9E20;
    }
}
