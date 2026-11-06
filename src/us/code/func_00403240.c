#include "common.h"

s32 CScene__UnmarkGeneratorsSaved(s32);

extern s32 D_800F7078;

void MantidQueen_EnterMode14(void) {
    CScene__UnmarkGeneratorsSaved((s32) &D_800F7078);
}
