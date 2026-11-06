#include "common.h"
#include "structs/libaudio.h"

/* libultra's __allocParam (synthesizer.c): take an ALParam off the free list
 * of alGlobals->drvr. */
extern ALGlobals *alGlobals; /* alGlobals */

ALParam *__allocParam(void) {
    ALParam *head;
    ALParam *update;

    head = alGlobals->drvr.paramList;
    update = NULL;
    if (head != NULL) {
        update = head;
        alGlobals->drvr.paramList = update->next;
        update->next = NULL;
    }
    return update;
}
