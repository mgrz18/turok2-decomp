#include "common.h"

s32 CScene__UnmarkGeneratorsSaved(s32);

extern s32 D_800F7078;

void TotemDefense_StopGenerators(void) {
    CScene__UnmarkGeneratorsSaved((s32) &D_800F7078);
}
