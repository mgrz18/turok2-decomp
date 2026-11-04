#include "common.h"
#include "structs/controller.h"
#include "structs/demo.h"


void func_0042F344(void);

static __inline__ s32 func_0027BC34(void) {
    DemoPlayer *state = &D_800F56B0;

    if (state->unk4 != 0 || state->playing != 0) {
        return state->playerCount;
    }
    return 0;
}

/* Reads the controllers and works out which buttons were just pressed. */
void func_0027BB80(void) {
    s32 i;

    if (func_0027BC34()) {
        func_0042F344();
        for (i = 0; i < 4; i++) {
            D_800F5EC0[i].buttonsSingleNoHold = 0;
            D_800F5EC0[i].buttonsDouble = 0;
            D_800F5EC0[i].buttonsRepeat = 0;
            D_800F5EC0[i].buttonsSingle = (D_800F5EC0[i].previousButtons ^ D_800F5EC0[i].buttons) & D_800F5EC0[i].buttons;
        }
    }
}
