#ifndef GLOBALS_ENGINE_H
#define GLOBALS_ENGINE_H

/* The engine's frame timing, mode and thread globals (Turok 3's tengine.c
 * names where it has them), 0x800B6D14-0x800B6E98. */

#include "common.h"

/* Frame timing, advanced by CEngineApp__AdvanceFrameData / CEngineApp__Retrace. */
extern s32 gFirstFrame;          /* 1 until the first retrace ("firstFrame") */
extern u32 gFrameCount;          /* frames drawn ("frame_number") */
extern s32 gEvenOdd;             /* 0/1: which display list, matrix and frame buffer half ("even_odd") */
extern u32 gFrameCountGameplay;  /* frames of gameplay ("game_frame_number") */
extern s32 gRetraceCount;        /* retraces ("retrace_count") */
extern f32 gFrameIncrement;      /* the frame's time step, in 15ths of a second ("frame_increment") */
extern f32 gEnemyTimeScale;      /* scales gFrameIncrement for the enemies (1.0) */
extern f32 gParticleTimeScale;   /* scales gFrameIncrement for the particles (1.0) */
extern f32 gEmitterTimeScale;    /* scales the emitters' spin step (1.0) */

/* Profiling, in seconds. */
extern f32 gSchedRspTime;        /* gfx task start to RSP done */
extern f32 gSchedRspTotalTime;   /* RSP gfx time summed over yields */
extern f32 gSchedRdpTime;        /* RDP time of the gfx task */
extern f32 gRenderTime;          /* CEngineApp__Draw building the display list */

#endif
