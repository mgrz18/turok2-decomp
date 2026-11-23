#include "common.h"
#include "structs/controller.h"
#include "structs/demo.h"

void Demo_UpdateStreams(void);

static __inline__ s32 Demo_GetPlayerCount_27BB80(void) {
    DemoPlayer *state = &gDemoPlayer;

    if (state->unk4 != 0 || state->playing != 0) {
        return state->playerCount;
    }
    return 0;
}

/* Reads the controllers and works out which buttons were just pressed. */
void Demo_UpdateInput(void) {
    s32 i;

    if (Demo_GetPlayerCount_27BB80()) {
        Demo_UpdateStreams();
        for (i = 0; i < 4; i++) {
            gControllers[i].buttonsSingleNoHold = 0;
            gControllers[i].buttonsDouble = 0;
            gControllers[i].buttonsRepeat = 0;
            gControllers[i].buttonsSingle = (gControllers[i].previousButtons ^ gControllers[i].buttons) & gControllers[i].buttons;
        }
    }
}

/* gDemoPlayer's value at 0x1B4 while either of its flags at 4 and 8 is set,
 * else 0. */
s32 Demo_GetPlayerCount(void) {
    DemoPlayer *state = &gDemoPlayer;

    if (state->unk4 != 0 || state->playing != 0) {
        return state->playerCount;
    }
    return 0;
}

#include "globals/frame.h"
#include "globals/demo.h"
#include "protos/cache.h"

/* At gDemoPlayer + 0x194. */
/* The ROM reaches these two through their own symbols, not gDemoPlayer. */


static __inline__ s32 Demo_GetPlayerCount_27BC68(void) {
    DemoPlayer *state = &gDemoPlayer;

    if (state->unk4 != 0 || state->playing != 0) {
        return state->playerCount;
    }
    return 0;
}

void Demo_AdvanceMatchStream(void) {
    DemoPlayer *state;
    DemoStream *timer;
    s32 flag;

    if (Demo_GetPlayerCount_27BC68()) {
        CCache__SetFrameDelay(8);
        flag = gDemoPlayer_fastForward;
        state = &gDemoPlayer;
        timer = &state->match;
        if (flag != 0) {
            gDemoPlayer_match_decode(timer);
        } else if (--timer->frames <= 0) {
            timer->decode(timer);
        }
        gNextTicks = state->match.value;
    }
}
