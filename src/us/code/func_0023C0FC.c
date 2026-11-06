#include "common.h"
#include "structs/collision.h"

/* Clears a sweep's hit: no hit yet, the whole move to go. */
void SweepHit_Clear(SweepHit *hit) {
    hit->type = 0;
    hit->posOut = NULL;
    hit->regionOut = NULL;
    hit->kindOut = NULL;
    hit->dataOut = NULL;
    hit->fraction = 1.0f;
}
