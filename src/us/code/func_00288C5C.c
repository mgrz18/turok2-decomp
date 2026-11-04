#include "common.h"
#include "structs/game.h"

/* The game state (CEngineApp in func_00286440). */

void func_00288C5C(CEngineApp *arg0, s32 arg1) {
    arg0->m_Settings.fadeMode = 2;
    arg0->m_Settings.nextMode = arg1;
}
