#include "common.h"
#include "structs/game_object.h"

s32 Door_Step(s32, s32);
extern s32 D_800B2290;

void Door_Setup(s32 arg0, CAI *ai) {
    ai->m_pModeTable = (CAIModeInfo *) &D_800B2290;
    ai->m_pfStep = Door_Step;
}
