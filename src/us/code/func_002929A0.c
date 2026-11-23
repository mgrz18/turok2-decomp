#include "common.h"
#include "structs/libaudio.h"

/* libaudio (seqchanstate.c): a channel back to its defaults. */
void __resetPerfChanState(ALSeqPlayer *seqp, s32 chan) {
    seqp->chanState[chan].fxId = 0;             /* AL_FX_NONE */
    seqp->chanState[chan].fxmix = 0;
    seqp->chanState[chan].pan = 0x40;           /* AL_PAN_CENTER */
    seqp->chanState[chan].vol = 0x7F;           /* AL_VOL_FULL */
    seqp->chanState[chan].priority = 5;         /* AL_DEFAULT_PRIORITY */
    seqp->chanState[chan].sustain = 0;
    seqp->chanState[chan].bendRange = 200;
    seqp->chanState[chan].pitchBend = 1.0f;
}
