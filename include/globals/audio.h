#ifndef GLOBALS_AUDIO_H
#define GLOBALS_AUDIO_H

/* The sound world (Turok 3's audioworld.c object) and the audio manager's
 * statics (the SDK audiomgr.c pattern: amCreateAudioMgr, __amMain, __amDMA). */

#include "common.h"
#include "ultra_types.h"
#include "structs/libaudio.h"
#include "structs/sound.h"
#include "structs/sound_kind.h"

extern CAudioWorld gAudioWorld;
extern s32 gAudioRomBase;                 /* the ROM address of the audio data */
extern OSMesg gAudioFrameMsgBuf[10];      /* __am.audioFrameMsgBuf */
extern OSMesg gAudioReplyMsgBuf[10];      /* __am.audioReplyMsgBuf */
extern OSThread gAudioThread;             /* __am.thread */
extern OSMesgQueue gAudioDmaMessageQ;     /* audDMAMessageQ */
extern OSMesgQueue gAudioFrameMsgQ;       /* __am.audioFrameMsgQ */
extern OSMesgQueue gAudioReplyMsgQ;       /* __am.audioReplyMsgQ */
extern ALGlobals gAudioGlobals;           /* __am.g */
extern s32 gAudioFrameSize;               /* samples per frame rounded up to 16 */
extern s32 gAudioMaxRSPCmds;
extern ALHeap *gAudioHeap;                /* the DMA cache entries' heap */
extern OSMesg gAudioDmaMessageBuf[0x3C];  /* audDMAMessageBuf */
extern s16 gSndVolumeCurve[];             /* SndVoicePool_Construct builds it */

extern char sCSPlayerFile[17];      /* "audio/csplayer.c", libaudio csplayer.c's __assert file name */

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
