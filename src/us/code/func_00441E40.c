#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"



void TotemDefense_StopGenerators_2(void) {
    CScene__UnmarkGeneratorsSaved((s32) &gScene);
}
