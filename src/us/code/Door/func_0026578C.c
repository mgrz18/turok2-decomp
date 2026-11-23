#include "common.h"
#include "structs/event.h"
#include "globals/scene.h"
#include "protos/scene.h"


void Door_EventOpenRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 1);
}
