#include "common.h"
#include "structs/controller.h"
#include "structs/demo.h"


void Demo_UpdateStreams(void);

static __inline__ s32 Demo_GetPlayerCount(void) {
    DemoPlayer *state = &D_800F56B0;

    if (state->unk4 != 0 || state->playing != 0) {
        return state->playerCount;
    }
    return 0;
}

/* Reads the controllers and works out which buttons were just pressed. */
void Demo_UpdateInput(void) {
    s32 i;

    if (Demo_GetPlayerCount()) {
        Demo_UpdateStreams();
        for (i = 0; i < 4; i++) {
            D_800F5EC0[i].buttonsSingleNoHold = 0;
            D_800F5EC0[i].buttonsDouble = 0;
            D_800F5EC0[i].buttonsRepeat = 0;
            D_800F5EC0[i].buttonsSingle = (D_800F5EC0[i].previousButtons ^ D_800F5EC0[i].buttons) & D_800F5EC0[i].buttons;
        }
    }
}
