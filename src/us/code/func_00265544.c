#include "common.h"
#include "structs/game_object.h"

s32 func_002655C4(s32, s32);
extern s32 D_800B2290;

void func_00265544(s32 arg0, CAI *ai) {
    ai->m_pModeTable = (CAIModeInfo *) &D_800B2290;
    ai->m_pfStep = func_002655C4;
}
