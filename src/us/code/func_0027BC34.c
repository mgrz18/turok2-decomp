#include "common.h"
#include "structs/demo.h"


/* D_800F56B0's value at 0x1B4 while either of its flags at 4 and 8 is set,
 * else 0. */
s32 func_0027BC34(void) {
    DemoPlayer *state = &D_800F56B0;

    if (state->unk4 != 0 || state->playing != 0) {
        return state->playerCount;
    }
    return 0;
}
