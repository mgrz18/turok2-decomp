/* libaudio's sound player (lib/ultralib sndp*.c, PR/libaudio.h), with the SDK's types, for the
 * game's sound voices (SndVoice; round 28, part q2). ALSndId is an s16, ALPan a u8. */

#ifndef PROTOS_SNDP_H
#define PROTOS_SNDP_H

#include "common.h"
#include "structs/libaudio.h"

s16 alSndpAllocate(ALSndPlayer *sndp, ALSound *sound);
void alSndpDeallocate(ALSndPlayer *sndp, s16 id);
void alSndpSetSound(ALSndPlayer *sndp, s16 id);
void alSndpPlay(ALSndPlayer *sndp);
void alSndpStop(ALSndPlayer *sndp);
void alSndpSetVol(ALSndPlayer *sndp, s16 vol);
void alSndpSetPitch(ALSndPlayer *sndp, f32 pitch);
void alSndpSetPan(ALSndPlayer *sndp, u8 pan);
void alSndpSetPriority(ALSndPlayer *sndp, s16 id, u8 priority);
void alSndpSetFXMix(ALSndPlayer *sndp, u8 mix);
s32 alSndpGetState(ALSndPlayer *sndp);

#endif
