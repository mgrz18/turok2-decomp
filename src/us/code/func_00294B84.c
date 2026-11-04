#include "common.h"
#include "structs/libaudio.h"

extern ALGlobals *D_800B7760; /* alGlobals */

/* libultra's __freeParam (synthesizer.c): push an ALParam back onto the free
 * list of alGlobals->drvr. */
void func_00294B84(ALParam *param) {
    ALSynth *drvr = &D_800B7760->drvr;

    param->next = drvr->paramList;
    drvr->paramList = param;
}
