#include "common.h"
#include "structs/event.h"

void CScene__SetInstanceSavedBitInLevel(void *, u16, u16, s32);          /* extern */
extern u8 D_800F7078[];

void Generator_EventOffRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(D_800F7078, arg0->level, arg0->actor, 0);
}
