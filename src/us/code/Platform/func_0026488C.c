#include "common.h"
#include "globals/scene.h"
#include "structs/event.h"
#include "protos/scene.h"


void Platform_EventOnRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 1);
}
