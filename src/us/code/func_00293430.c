#include "common.h"
#include "structs/libaudio.h"

void alSynDelete(ALSynth *drvr); /* alSynDelete */
extern ALGlobals *D_800B7760; /* alGlobals */

/* libultra's alClose (audio.c). The ROM keeps &alGlobals in a register. */
void alClose(ALGlobals *glob) {
    ALGlobals **g = &D_800B7760;

    if (*g != NULL) {
        alSynDelete(&glob->drvr);
        *g = NULL;
    }
}
