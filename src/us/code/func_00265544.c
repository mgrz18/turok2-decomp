#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

s32 Door_Step(s32, s32);

void Door_Setup(s32 arg0, CAI *ai) {
    ai->m_pModeTable = gDoorModeTable;
    ai->m_pfStep = Door_Step;
}
