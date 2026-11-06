#include "common.h"
#include "structs/demo.h"


/* gDemoPlayer's value at 0x1B4 while either of its flags at 4 and 8 is set,
 * else 0. */
s32 Demo_GetPlayerCount(void) {
    DemoPlayer *state = &gDemoPlayer;

    if (state->unk4 != 0 || state->playing != 0) {
        return state->playerCount;
    }
    return 0;
}
