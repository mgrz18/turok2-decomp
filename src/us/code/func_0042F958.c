#include "common.h"
#include "structs/demo.h"

extern s32 D_8011AD10;

void Demo_UpdateMatchStream(void) {
    DemoPlayer *s = &D_800F56B0;
    DemoStream *t = &s->match;

    if (s->fastForward != 0 || --t->frames <= 0) {
        t->decode(t);
    }
    D_8011AD10 = s->match.value;
}
