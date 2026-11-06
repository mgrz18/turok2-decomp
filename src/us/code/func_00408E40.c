#include "common.h"
#include "globals/scene.h"

s32 CScene__UnmarkGeneratorsSaved(s32);


void TotemDefense_StopGenerators(void) {
    CScene__UnmarkGeneratorsSaved((s32) &gScene);
}
