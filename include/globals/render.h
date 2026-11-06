#ifndef GLOBALS_RENDER_H
#define GLOBALS_RENDER_H

/* The screen, the video modes and the frame buffers (gDList is gfx.h's,
 * gVideoModes / gVideoModesPAL frame.h's). */

#include "common.h"

extern s32 gScreenWidth;            /* Turok 3's screen_width */
extern s32 gScreenHeight;           /* Turok 3's screen_height */
extern s32 gResolution;             /* the resolution set, index in gVideoModes */
extern s32 gAppliedResolution;      /* the resolution last applied */
extern s32 gVideoClearFrames;       /* frames left to clear the screen around a mode change */
extern s32 gVideoModeCounts[2];     /* resolutions per TV region */
extern u32 gTotalFramebuffers;      /* 3 */
extern u32 gDisplayListSize;        /* in commands */
extern u32 gLineListSize;           /* in commands */
extern u32 gNextFrameToShow;        /* wraps at gTotalFramebuffers */

#endif
