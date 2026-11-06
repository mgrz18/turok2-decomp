#ifndef GLOBALS_FRAME_H
#define GLOBALS_FRAME_H

/* The engine's frame loop and scheduler state (Turok 3's tengine.c), the
 * display list and frame buffers, the crash screen. */

#include "structs/frame.h"

extern CFrameData *gFrameData;           /* the frame being finished */
extern u64 gLastEngineUpdate;
extern s32 gNextTicks;
extern s32 gDisplayListCount;
extern s32 gNumTicksPending;
extern s32 gNumTicksWaiting;
extern u16 *gPendingFB;                  /* the frame buffer just swapped in (globals/framebuffer.h) */
extern s32 gWarmBoot;                    /* set on an NMI reset */

/* Task timing (the profiler). */
extern u64 sAudioTaskStartTime;
extern f32 sGfxRspTime;
extern u64 sGfxRspResumeTime;
extern s64 sRdpStartTime;
extern s64 sGfxTaskStartTime;

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
