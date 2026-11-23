#include "common.h"
#include "structs/libaudio.h"

/* libaudio (seqchanstate.c): a channel takes an instrument and its pan, volume, priority and bend range. */
void __setInstChanState(ALSeqPlayer *seqp, ALInstrument *inst, s32 chan) {
    seqp->chanState[chan].instrument = inst;
    seqp->chanState[chan].pan = inst->pan;
    seqp->chanState[chan].vol = inst->volume;
    seqp->chanState[chan].priority = inst->priority;
    seqp->chanState[chan].bendRange = inst->bendRange;
}
