#include "common.h"
#include "globals/scene.h"
#include "structs/event.h"

s32 CScene__SetInstanceSavedBitInLevel(CScene *, u16, u16, s32); /* extern */

void Platform_EventOnRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 1);
}
