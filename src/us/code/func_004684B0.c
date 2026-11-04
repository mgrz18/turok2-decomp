#include "common.h"
#include "structs/demo.h"

extern void *D_00438408[]; /* NULL-terminated list */
extern void func_0042EE4C(void *entry);

void func_004684B0(void) {
    DemoPlayer *s = &D_800F56B0;

    if (s->active) {
        func_0042EE4C(D_00438408[s->index]);
        s->index++;
        if (D_00438408[s->index] == NULL) {
            s->index = 0;
        }
    }
}
