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

/* The engine modes (CEngineApp.m_Settings.mode, EngineModeNN_*): what
 * CEngineApp__SetMode switches between. */
struct CEngineApp;
typedef struct EngineMode {
    /* 0x0 */ void (*init)(struct CEngineApp *app);    /* on entering the mode (EngineMode17_Switch_Retrace) */
    /* 0x4 */ void (*retrace)(struct CEngineApp *app); /* every retrace (CEngineApp__Retrace) */
    /* 0x8 */ void (*draw)(struct CEngineApp *app);    /* every frame (CEngineApp__Draw) */
} EngineMode; /* size 0xC */
extern EngineMode gEngineModeTable[18];

extern s32 gPersistentDataLoaded;   /* the level's persistent data is loaded (CScene__InitPersistentData) */
extern s32 gAttractRunCount;        /* attract-mode runs (cinema, demo) */
extern s32 gPakCheckStarted;        /* EngineMode00_PakCheck has started */
extern s32 gShowExpansionPakScreen; /* the legal screen goes on to the expansion pak screen (1) */
extern s32 gLegalScreenNoSkip;      /* the legal screen cannot be skipped yet (1) */
extern s32 gFinalEnding;            /* 0: EngineMode15_Ending, 1: EngineMode16_Ending */
extern s32 gEndingPrimagenDefeated; /* the first player beat the Primagen (for the ending) */

/* The scheduler (__scMain and its handlers). */
extern s32 sSchedGfxWatchdog;          /* retraces since the last gfx task finished (crashes at 1200) */
extern s32 gDecompressorBoostRetraces; /* retraces left at gThreadPriorityDecompressorHigh */
extern s32 sSchedResetPressed;         /* the pre-NMI message came */
extern s32 sSchedResetBlankRetraces;   /* retraces after reset before the screen is blanked (20) */

/* Thread priorities (Turok 3's PRIORITY_*). */
extern s32 gThreadPriorityIdle;
extern s32 gThreadPriorityIdleInit;
extern s32 gThreadPriorityDecompressorLow;
extern s32 gThreadPriorityMain;
extern s32 gThreadPriorityDecompressorHigh;
extern s32 gThreadPriorityCache;
extern s32 gThreadPriorityAudio;
extern s32 gThreadPriorityLoader;
extern s32 gThreadPriorityScheduler;

extern char gBootSignature[0x10]; /* "sexysteve" while warm-booted ("signature") */

#endif
