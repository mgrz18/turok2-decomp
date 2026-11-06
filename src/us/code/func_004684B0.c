#include "common.h"
#include "structs/demo.h"
#include "globals/demo.h"

extern void Demo_Start_2(DemoHeader *entry);

void Demo_PlayNext_2(void) {
    DemoPlayer *s = &gDemoPlayer;

    if (s->active) {
        Demo_Start_2(gDemoTable_2[s->index]);
        s->index++;
        if (gDemoTable_2[s->index] == NULL) {
            s->index = 0;
        }
    }
}
