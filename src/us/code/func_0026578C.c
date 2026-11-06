#include "common.h"
#include "structs/event.h"
#include "globals/scene.h"

s32 CScene__SetInstanceSavedBitInLevel(s32 *, u16, u16, s32); /* extern */

void Door_EventOpenRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 1);
}
