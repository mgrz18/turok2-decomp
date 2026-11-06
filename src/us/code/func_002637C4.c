#include "common.h"
#include "structs/event.h"

void CScene__SetInstanceSavedBitInLevel(void *, u16, u16, s32);          /* extern */
extern u8 gScene[];

void Generator_EventOffRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(gScene, arg0->level, arg0->actor, 0);
}
