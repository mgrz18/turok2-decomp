#ifndef GLOBALS_AUDIO_H
#define GLOBALS_AUDIO_H

/* The sound world (Turok 3's audioworld.c object) and the audio manager's
 * statics (the SDK audiomgr.c pattern: amCreateAudioMgr, __amMain, __amDMA). */

#include "common.h"
#include "ultra_types.h"
#include "structs/libaudio.h"
#include "structs/sound.h"

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

#endif
