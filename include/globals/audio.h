#ifndef GLOBALS_AUDIO_H
#define GLOBALS_AUDIO_H

#include "common.h"
#include "structs/libaudio.h"
#include "structs/sound_kind.h"

/* SDK audio manager (audiomgr.c statics) */
extern u32 audFrameCt;            /* audio frame counter */
extern u32 nextDMA;               /* DMAs issued this frame */
extern u32 curAcmdList;           /* command list alAudioFrame builds (0/1) */
extern s32 sAmFramesHandled;
extern u32 sAmBusyTime;
extern s32 sAudioDMACacheBlocks;

/* CAudioWorld, voices, sequences */
extern s32 gAudioWorldReady;
extern s32 sAudioWorldReverbParams[51];   /* ALSynConfig.params, AL_FX_CUSTOM */
extern SoundKindVolume sSoundKindVolumeTable[31];
extern f32 sSndVoiceRampStep;
extern f32 sPitchRatioTable[121];         /* 2^(cents/1200) every 60 cents from -6000 */
extern f32 sSoundHearingRangeSq;
extern s32 sSndVoicesActive;
extern s32 sSndVoicesPeak;
extern s32 sSndSeqCurrentId;
extern s32 sSndSeqLoadRetries;
extern s32 sLoopSoundsLastStarted;
extern s32 sLoopSoundsLastStopped;
extern s32 sLoopSoundsLastPaused;
extern s32 sLoopSoundsLastResumed;
extern ALSound sSpeechSound;

#endif
