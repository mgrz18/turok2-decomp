#ifndef GLOBALS_FRAME_H
#define GLOBALS_FRAME_H

/* The engine's frame loop and scheduler state (Turok 3's tengine.c), the
 * display list and frame buffers, the crash screen. */

#include "ultra_types.h"
#include "structs/frame.h"
#include "structs/sched.h"
#include "structs/gbi.h" /* Mtx */

extern CFrameData *gFrameData;           /* the frame being finished */
extern u64 gLastEngineUpdate;
extern s32 gNextTicks;
extern s32 gDisplayListCount;
extern s32 gCurrentTick;                 /* retraces since the last swap ("c_field") */
extern s32 gNumTicks0;                   /* the retraces of the last four frames ("n_fields" ...) */
extern s32 gNumTicks2;                   /* "n_ll_fields" */
extern s32 gNumTicks3;                   /* "n_lll_fields" */
extern s32 gNumTicksPending;
extern s32 gNumTicksWaiting;
extern s32 gCurrentTickWaiting;          /* the write index of gWaitingFrames ("c_waiting") */
extern CFrameData *gWaitingFrames[3];    /* frames done, not shown yet */
extern s32 gPendingFB;
extern s32 gWarmBoot;                    /* set on an NMI reset */

/* Task timing (the profiler). */
extern u64 sAudioTaskStartTime;
extern f32 sGfxRspTime;
extern u64 sGfxRspResumeTime;
extern s64 sRdpStartTime;
extern s64 sGfxTaskStartTime;

/* The engine thread's message queue ("gfxFrameMsgQ", "gfxFrameMsgBuf"). */
extern OSMesgQueue gGfxFrameMessageQueue;
extern OSMesg gGfxFrameMessageBuffer[0x1000];
extern OSScClient gGfxClient;             /* the engine thread, osScAddClient'ed at boot ("gfxClient") */
extern OSScMsg *gGfxMsg;                 /* the message CEngineApp__Main received ("pMsg") */

/* The thread stacks and threads (Turok 3's names). */
extern u64 gGfxYieldBuffer[];            /* the RSP yield buffer (the start of the cleared BSS) */
extern u8 gThreadStackAudio[0x2800];
extern u8 gThreadStackIdle[0x80];
extern u8 gThreadStackMain[0x8000];
extern u8 gSchedulerStack[0x2000];
extern OSThread gThreadIdle;
extern OSThread gThreadMain;

/* The identity matrix, guMtxIdent at boot (CEngineApp__Construct); the emitters load it. */
extern Mtx gIdentityMtx;

/* The RDP fifo (CEngineApp__SubmitGfxTask's output buffer). */
extern s32 gRspFifoSize;                 /* 0x8000 */
extern u64 *gRspFifoBuffer;

/* The pool buffers (addresses, set at boot). */
extern s32 gAudioHeapBase;
extern s32 gAudioHeapSize;
extern s32 gLineList0;
extern s32 gLineList1;
extern s32 gDisplayList0;
extern s32 gDisplayList1;
extern s32 gDepthBuffer;

/* The crash screen. */
extern u16 sCrashTextColor;

#endif
