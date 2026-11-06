#include "common.h"
#include "structs/demo.h"
#include "globals/demo.h"

s32 Demo_Start(DemoHeader *);

void Demo_PlayNext(void) {
    DemoPlayer *state = &gDemoPlayer;
    s32 temp_v0;

    if (state->active != 0) {
        Demo_Start(gDemoTable[state->index]);
        temp_v0 = state->index + 1;
        state->index = temp_v0;
        if (gDemoTable[temp_v0] == NULL) {
            state->index = 0;
        }
    }
}
