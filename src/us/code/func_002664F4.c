#include "common.h"
#include "globals/scene.h"

/* Where an actor was placed: CScene__SetInstanceSavedBitInLevel(world, level, index, 1) sets its bit
 * in the per-level bit sections of the world. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 actorIndex;
    /* 0x06 */ u8 pad6[4];
    /* 0x0A */ u16 level;
} ActorPlacement;

s32 CScene__SetInstanceSavedBitInLevel(s32 *, u16, u16, s32); /* extern */

void Action_EventGoRemote(ActorPlacement *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actorIndex, 1);
}
