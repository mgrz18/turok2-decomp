/* The second VM module's copy of src/us/vm1/Demo/func_0042F958.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/demo.h"
#include "globals/frame.h"


void func_00468878(void) {
    DemoPlayer *s = &gDemoPlayer;
    DemoStream *t = &s->match;

    if (s->fastForward != 0 || --t->frames <= 0) {
        t->decode(t);
    }
    gNextTicks = s->match.value;
}
