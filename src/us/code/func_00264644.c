/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00207C94.c, func_00207C94, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    u8 pad0[0x24];
    u32 flags; /* 0x24 */
    u8 pad28[0x24];
    f32 unk4C;
    f32 delay; /* 0x50 */
} StateInfo;

typedef struct {
    u32 flags; /* 0x0 */
    u8 pad4[0x3C];
    f32 timer; /* 0x40 */
} MotionState;

s32 func_00243414(void *actor, void *state, s32 mode);

void func_00264644(u8 *actor, MotionState *state) {
    StateInfo *info = (StateInfo *) (*(u8 **) (actor + 0x14) + 0x14);
    s32 ok = 1;

    if (info->flags & 0x20) {
        u32 f = state->flags & 0x10000000;
        ok = f != 0;
    }
    if (info->flags & 0x200) {
        if (!(state->flags & 0x20000)) {
            ok = 0;
        }
    }
    if (info->flags & 0x800) {
        if (state->flags & 0x20000) {
            ok = 0;
        }
    }
    if (ok != 0) {
        if (state->timer >= info->delay) {
            func_00243414(actor, state, 3);
        }
    } else {
        state->timer = 0.0f;
    }
}
