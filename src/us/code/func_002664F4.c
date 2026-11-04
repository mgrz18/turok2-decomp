#include "common.h"

/* Where an actor was placed: func_002245BC(world, level, index, 1) sets its bit
 * in the per-level bit sections of the world. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 actorIndex;
    /* 0x06 */ u8 pad6[4];
    /* 0x0A */ u16 level;
} ActorPlacement;

s32 func_002245BC(s32 *, u16, u16, s32); /* extern */
extern s32 D_800F7078;

void func_002664F4(ActorPlacement *arg0) {
    func_002245BC(&D_800F7078, arg0->level, arg0->actorIndex, 1);
}
