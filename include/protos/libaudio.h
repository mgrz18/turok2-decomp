/* libaudio functions the game calls, with ultralib's prototypes
 * (lib/ultralib/include/PR/libaudio.h); the types are structs/libaudio.h's. */

#ifndef PROTOS_LIBAUDIO_H
#define PROTOS_LIBAUDIO_H

#include "common.h"
#include "structs/libaudio.h"

void alCSeqNew(ALCSeq *seq, u8 *ptr);
void alCSPSetSeq(ALCSPlayer *seqp, ALCSeq *seq);
void alCSPPlay(ALCSPlayer *seqp);
void alCSPStop(ALCSPlayer *seqp);
s32 alCSPGetState(ALCSPlayer *seqp);
void alCSPSetBank(ALCSPlayer *seqp, ALBank *b);
void alCSPSetVol(ALCSPlayer *seqp, s16 vol);
void alCSPSetChlFXMix(ALCSPlayer *seqp, u8 chan, u8 fxmix);

#endif
