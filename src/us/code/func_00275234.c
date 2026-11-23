#include "common.h"
#include "structs/sound.h"

/* Ends a duck: the music stops sliding and returns to full volume. */
void SndSeq_Unduck(SndSeq *arg0) {
    arg0->sliding = 0;
    arg0->target = 1.0f;
}
