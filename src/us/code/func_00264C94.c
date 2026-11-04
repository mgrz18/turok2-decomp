#include "common.h"
#include "structs/game_object.h"

s32 func_00264994(s32, s32);
s32 func_00264CEC(s32, s32);
s32 func_00264D98(s32, s32);

extern s32 D_800B21E0;

void func_00264C94(s32 arg0, CAI *ai) {
    ai->m_pModeTable = (CAIModeInfo *) &D_800B21E0;
    ai->m_pfStep = func_00264D98;
    ai->m_pfCollision = func_00264CEC;
    ai->m_pfCalculateOrientationMatrix = (void *) (func_00264994 + 0xC);
}
