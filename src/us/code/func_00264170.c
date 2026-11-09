#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"


void Platform_IsOn(s32 arg0) {
    CScene__GetInstanceSavedBit(&gScene, arg0);
}
