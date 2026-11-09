/* t2port.h: the platform side of the native port's runtime (docs/port/LINK.md).
 *
 * port/runtime/t2ultra.c replaces the libultra device layer the game calls (VI, SP/DP, AI, PI,
 * SI/controllers, timers, caches, TLB, FPU) on top of t2os (threads/queues/events), and
 * port/runtime/t2boot.c replaces the N64 boot path. The executable (port/app/main.cpp) drives
 * it with this API, one call per host frame, from its own (platform) thread:
 *
 *   T2Port_Boot(&cfg);                  arena, ROM, t2os, the main game thread (mainproc)
 *   while (running) {
 *       T2Port_Frame();                 completions, timers, one VI retrace, run the game
 *                                       threads until all of them are blocked
 *       dl = T2Port_TakeGfxTask(...);   the display list the scheduler started, if any
 *   }
 *
 * Plain C, no libultraship dependency: the controller hooks are installed by the app. */
#ifndef T2PORT_H
#define T2PORT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct T2PortConfig {
    const char *romPath;    /* baserom.us.z64 for PI DMA (osPiRawStartDma); NULL: DMA reads zeros */
    uint32_t arenaSize;     /* the game heap ("RDRAM arena"); 0: 32 MB */
    int lowMemory;          /* Linux bring-up aid: arena and game thread stacks below 2 GB (see LINK.md) */
    int verbose;            /* log every device call once / every task */
    int audio;              /* --audio: Sound__Initialize runs, the audio manager makes one frame per
                               retrace through the ABI1 interpreter (port/audio, docs/port/AUDIO.md) */
    void (*mainEntry)(void *); /* tests: run this instead of mainproc on the main game thread */
} T2PortConfig;

/* Controller hooks (LUS's ControlDeck, set by the app; NULL: no controller). bits gets one bit
 * per connected pad; pads points at 4 OSContPad (6 bytes each, N64 layout). */
typedef struct T2PortContHooks {
    int (*init)(uint8_t *bits);
    void (*read)(void *pads);
} T2PortContHooks;
void T2Port_SetContHooks(const T2PortContHooks *hooks);

int T2Port_Boot(const T2PortConfig *cfg);
void T2Port_Frame(void);

typedef struct T2PortTask {
    uint32_t type;          /* 1 gfx (M_GFXTASK), 2 audio (M_AUDTASK) */
    uint32_t flags;
    const void *task;       /* the OSTask the scheduler started */
    const void *dataPtr;    /* task->t.data_ptr: the display list / Acmd list */
    uint32_t dataSize;
} T2PortTask;
/* The gfx task started during the last T2Port_Frame (0: none). Its SP/DP completions are
 * posted at the start of the next T2Port_Frame. */
int T2Port_TakeGfxTask(T2PortTask *out);

typedef struct T2PortStats {
    uint64_t frames;        /* T2Port_Frame calls (VI retraces) */
    uint64_t switches;      /* t2os thread switches */
    uint64_t gfxTasks, audioTasks, otherTasks;
    uint64_t piDmas, piBytes;
    uint64_t viSwaps;
    uint64_t siPosts;
    uint64_t missingCalls;  /* calls into placeholder functions (t2_missing) */
} T2PortStats;
void T2Port_GetStats(T2PortStats *out);

/* Called by the generated placeholder functions (port/runtime/t2_placeholders.c): logs the
 * name once, counts, aborts if T2_MISSING_ABORT is set in the environment. */
void t2_missing(const char *name);
/* Logs once per key (device calls the port does not model yet). */
void t2_log_once(const char *what);

/* Memory below 2 GB when lowMemory is set (else plain calloc). Never freed. */
void *t2_lowmem_alloc(size_t size);

/* ROM image (if one was given), for PI DMA. */
const uint8_t *t2_rom(size_t *size);

#ifdef __cplusplus
}
#endif

#endif
