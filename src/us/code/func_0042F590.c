#include "common.h"
#include "structs/demo.h"

s32 Demo_Start(s32);

extern s32 gDemoTable;

void Demo_PlayNext(void) {
    DemoPlayer *state = &gDemoPlayer;
    s32 temp_v0;

    if (state->active != 0) {
        Demo_Start(*(((__typeof__(&gDemoTable))((s8 *)&gDemoTable + (state->index * 4)))));
        temp_v0 = state->index + 1;
        state->index = temp_v0;
        if (*(((__typeof__(&gDemoTable))((s8 *)&gDemoTable + (temp_v0 * 4)))) == 0) {
            state->index = 0;
        }
    }
}
