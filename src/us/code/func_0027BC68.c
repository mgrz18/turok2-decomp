#include "common.h"
#include "structs/demo.h"


/* At D_800F56B0 + 0x194. */
/* The ROM reaches these two through their own symbols, not D_800F56B0. */
extern s32 D_800F56BC;                              /* D_800F56B0.fastForward */
extern void (*D_800F5858)(DemoStream *);         /* D_800F56B0.match.decode */
extern s32 gNextTicks;

void CCache__SetFrameDelay(s32);

static __inline__ s32 Demo_GetPlayerCount(void) {
    DemoPlayer *state = &D_800F56B0;

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
        flag = D_800F56BC;
        state = &D_800F56B0;
        timer = &state->match;
        if (flag != 0) {
            D_800F5858(timer);
        } else if (--timer->frames <= 0) {
            timer->decode(timer);
        }
        gNextTicks = state->match.value;
    }
}
