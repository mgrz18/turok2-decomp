#include "common.h"
#include "structs/libaudio.h"
#include "globals/libaudio.h"


/* libultra's __freeParam (synthesizer.c): push an ALParam back onto the free
 * list of alGlobals->drvr. */
void __freeParam(ALParam *param) {
    ALSynth *drvr = &alGlobals->drvr;

    param->next = drvr->paramList;
    drvr->paramList = param;
}
