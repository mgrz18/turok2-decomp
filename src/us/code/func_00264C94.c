#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

s32 func_00264994(s32, s32);
s32 Throwable_Collision(s32, s32);
s32 Throwable_Step(s32, s32);

void Throwable_Setup(s32 arg0, CAI *ai) {
    ai->m_pModeTable = gThrowableModeTable;
    ai->m_pfStep = Throwable_Step;
    ai->m_pfCollision = Throwable_Collision;
    ai->m_pfCalculateOrientationMatrix = (void *) (func_00264994 + 0xC);
}
