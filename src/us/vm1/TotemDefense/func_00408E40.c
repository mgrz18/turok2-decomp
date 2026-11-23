#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"



void TotemDefense_StopGenerators(void) {
    CScene__UnmarkGeneratorsSaved(&gScene);
}
