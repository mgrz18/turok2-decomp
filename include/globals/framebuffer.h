#ifndef GLOBALS_FRAMEBUFFER_H
#define GLOBALS_FRAMEBUFFER_H

/* The frame buffers (Turok 3's "cfb_16", "p_current_fb"). GfxPool_Alloc
 * carves gTotalFramebuffers of them out of the top of RDRAM; __scHandleRetrace
 * swaps gPendingFB (globals/frame.h) in as gCurrentFB. */

#include "common.h"

extern u16 *gCFB[];             /* gTotalFramebuffers frame buffers */
extern u16 *gCurrentFB;         /* the frame buffer on screen */

/* The word after gCurrentFB is the refresh rate, 50.0 or 60.0 (Turok 3's
 * gRefreshRate; CEngineApp__Main sets it from osTvType). It has no symbol of
 * its own: the code reaches it from gCurrentFB. */
#ifdef TARGET_PORT /* &gCurrentFB + 1 is past the host pointer: a variable of its own (port/game/t2sched.c) */
extern f32 t2_gRefreshRate;
#define gRefreshRate t2_gRefreshRate
#else
#define gRefreshRate (*(f32 *) (&gCurrentFB + 1))
#endif

#endif
