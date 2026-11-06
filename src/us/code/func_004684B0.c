#include "common.h"
#include "structs/demo.h"

extern void *D_00438408[]; /* NULL-terminated list */
extern void Demo_Start_2(void *entry);

void Demo_PlayNext_2(void) {
    DemoPlayer *s = &D_800F56B0;

    if (s->active) {
        Demo_Start_2(D_00438408[s->index]);
        s->index++;
        if (D_00438408[s->index] == NULL) {
            s->index = 0;
        }
    }
}
