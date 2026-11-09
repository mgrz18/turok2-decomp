#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"



void MantidQueen_EnterMode14(void) {
    CScene__UnmarkGeneratorsSaved((s32) &gScene);
}
