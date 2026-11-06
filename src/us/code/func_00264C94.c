#include "common.h"
#include "structs/game_object.h"

s32 func_00264994(s32, s32);
s32 Throwable_Collision(s32, s32);
s32 Throwable_Step(s32, s32);

extern s32 D_800B21E0;

void Throwable_Setup(s32 arg0, CAI *ai) {
    ai->m_pModeTable = (CAIModeInfo *) &D_800B21E0;
    ai->m_pfStep = Throwable_Step;
    ai->m_pfCollision = Throwable_Collision;
    ai->m_pfCalculateOrientationMatrix = (void *) (func_00264994 + 0xC);
}
