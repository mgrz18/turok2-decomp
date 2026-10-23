#include "common.h"

f32 func_00267648(void *actor, s32 arg);

/* Whether the actor stands on (or within 0.512 above) the ground. */
s32 func_002685F0(u8 *actor) {
    s32 state;

    if (actor[0] == 1) {
        if (*(s32 *) (actor + 0xD4) & 0x1000) {
            return 1;
        }
        state = *(s32 *) (actor + 0x140);
        if ((state & 0xC0000) && !(state & 0x40000)) {
            return 1;
        }
    }
    return *(f32 *) (actor + 0x8) - func_00267648(actor, 0) <= 0.512f;
}
