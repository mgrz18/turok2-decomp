#include "common.h"
#include "structs/controller.h"
#include "structs/demo.h"


void Demo_UpdateStreams(void);

static __inline__ s32 Demo_GetPlayerCount(void) {
    DemoPlayer *state = &gDemoPlayer;

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
            gControllers[i].buttonsSingleNoHold = 0;
            gControllers[i].buttonsDouble = 0;
            gControllers[i].buttonsRepeat = 0;
            gControllers[i].buttonsSingle = (gControllers[i].previousButtons ^ gControllers[i].buttons) & gControllers[i].buttons;
        }
    }
}
