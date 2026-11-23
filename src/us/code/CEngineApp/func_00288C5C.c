#include "common.h"
#include "structs/game.h"

/* The game state (CEngineApp in CEngineApp__Construct). */

void CEngineApp__SetupFadeTo(CEngineApp *arg0, s32 arg1) {
    arg0->m_Settings.fadeMode = 2;
    arg0->m_Settings.nextMode = arg1;
}
