#include "common.h"
#include "structs/libaudio.h"

/* libultra's __allocParam (synthesizer.c): take an ALParam off the free list
 * of alGlobals->drvr. */
extern ALGlobals *D_800B7760; /* alGlobals */

ALParam *__allocParam(void) {
    ALParam *head;
    ALParam *update;

    head = D_800B7760->drvr.paramList;
    update = NULL;
    if (head != NULL) {
        update = head;
        D_800B7760->drvr.paramList = update->next;
        update->next = NULL;
    }
    return update;
}
