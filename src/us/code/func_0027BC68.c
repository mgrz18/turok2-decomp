#include "common.h"
#include "structs/demo.h"
#include "globals/frame.h"


/* At gDemoPlayer + 0x194. */
/* The ROM reaches these two through their own symbols, not gDemoPlayer. */
extern s32 gDemoPlayer_fastForward;                              /* gDemoPlayer.fastForward */
extern void (*gDemoPlayer_match_decode)(DemoStream *);         /* gDemoPlayer.match.decode */

void CCache__SetFrameDelay(s32);

static __inline__ s32 Demo_GetPlayerCount(void) {
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

    if (Demo_GetPlayerCount()) {
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
