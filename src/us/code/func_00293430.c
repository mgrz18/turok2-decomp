#include "common.h"
#include "structs/libaudio.h"
#include "globals/libaudio.h"

void alSynDelete(ALSynth *drvr); /* alSynDelete */

/* libultra's alClose (audio.c). The ROM keeps &alGlobals in a register. */
void alClose(ALGlobals *glob) {
    ALGlobals **g = &alGlobals;

    if (*g != NULL) {
        alSynDelete(&glob->drvr);
        *g = NULL;
    }
}
