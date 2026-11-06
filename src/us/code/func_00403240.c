#include "common.h"

s32 CScene__UnmarkGeneratorsSaved(s32);

extern s32 gScene;

void MantidQueen_EnterMode14(void) {
    CScene__UnmarkGeneratorsSaved((s32) &gScene);
}
