#include "common.h"
#include "structs/demo.h"

extern void *gDemoTable_2[]; /* NULL-terminated list */
extern void Demo_Start_2(void *entry);

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
