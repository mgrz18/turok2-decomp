#include "common.h"
#include "structs/boss.h"

/* Starts a Blind One mover on a sequence of masks, one every interval frames. */
void BlindOne_StartMover_2(BlindOneMover *mover, s32 *seq, f32 interval) {
    mover->seq = seq;
    mover->pos = 0;
    mover->interval = interval;
    mover->timer = 0;
    mover->step = 0;
    mover->offset = 0;
}
