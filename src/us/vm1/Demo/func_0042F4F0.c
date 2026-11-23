#include "common.h"
#include "structs/demo.h"


void Demo_Reset(void) {
    gDemoPlayer.active = 0;
    gDemoPlayer.unk4 = 0;
    gDemoPlayer.playing = 0;
    gDemoPlayer.fastForward = 0;
    gDemoPlayer.index = 0;
    gDemoPlayer.saved = 0;
}
