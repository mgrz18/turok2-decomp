#include "common.h"
#include "structs/libaudio.h"

void func_00294150(ALSynth *drvr); /* alSynDelete */
extern ALGlobals *D_800B7760; /* alGlobals */

/* libultra's alClose (audio.c). The ROM keeps &alGlobals in a register. */
void func_00293430(ALGlobals *glob) {
    ALGlobals **g = &D_800B7760;

    if (*g != NULL) {
        func_00294150(&glob->drvr);
        *g = NULL;
    }
}
