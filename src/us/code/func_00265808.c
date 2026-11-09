#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"


s32 Door_IsOpen(s32 arg0) {
    return CScene__GetInstanceSavedBit(&gScene, arg0) == 1;
}
