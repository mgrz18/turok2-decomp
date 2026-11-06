#include "common.h"
#include "structs/libaudio.h"
#include "globals/libaudio.h"

void alSynNew(ALSynth *drvr, ALSynConfig *c);


/* libultra's alInit (audio.c). The ROM keeps &alGlobals in a register. */
void alInit(ALGlobals *g, ALSynConfig *c) {
    ALGlobals **glob = &alGlobals;

    if (*glob == NULL) {
        *glob = g;
        alSynNew(&g->drvr, c);
    }
}
