#include "common.h"
#include "globals/scene.h"

s32 CScene__UnmarkGeneratorsSaved(s32);


void MantidQueen_EnterMode14(void) {
    CScene__UnmarkGeneratorsSaved((s32) &gScene);
}
