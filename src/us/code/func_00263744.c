#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"


void Generator_EventOn(sptr arg0) {
    CScene__SetInstanceSavedBit(&gScene, arg0, 1);
}
