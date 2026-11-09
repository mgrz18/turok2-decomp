#include "common.h"
#include "m2c_macros.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/scene.h"


s32 func_0027AE00(s32 arg0) {
    s32 r;

    if (gPersistentDataLoaded != 0) {
        r = CScene__LoadGraphicSet(&gScene, arg0, 3, 1);
    } else {
        r = 0;
    }
    return r;
}
