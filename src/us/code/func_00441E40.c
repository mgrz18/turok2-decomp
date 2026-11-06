#include "common.h"

s32 CScene__UnmarkGeneratorsSaved(s32);

extern s32 gScene;

void TotemDefense_StopGenerators_2(void) {
    CScene__UnmarkGeneratorsSaved((s32) &gScene);
}
