#include "common.h"
#include "m2c_macros.h"
#include "globals/menu.h"
#include "globals/controller.h"
#include "globals/engine.h"

s32 CEngineApp__UpdateCinemaMode(s32, s32, s32);


void EngineMode16_Ending_Retrace(s32 arg0) {
    if (gFinalEnding != 0) {
        CEngineApp__UpdateCinemaMode(arg0, 1, 1);
        return;
    }
    if ((gMergedController_buttonsSingle & 0x1000) && (gCreditsDone == 0)) {
        gCreditsDone = 1;
        gMergedController_buttonsSingle = 0;
        gCreditsTimeLeft = 15.0f;
    }
}
