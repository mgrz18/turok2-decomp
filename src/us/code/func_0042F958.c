#include "common.h"
#include "structs/demo.h"
#include "globals/frame.h"


void Demo_UpdateMatchStream(void) {
    DemoPlayer *s = &gDemoPlayer;
    DemoStream *t = &s->match;

    if (s->fastForward != 0 || --t->frames <= 0) {
        t->decode(t);
    }
    gNextTicks = s->match.value;
}
