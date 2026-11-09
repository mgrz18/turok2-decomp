/* t2ultra.c: the libultra device layer of the native port (docs/port/LINK.md §3).
 *
 * The game's own libultra C for these functions drives N64 hardware registers (0xA4xxxxxx);
 * those files are left out of the link (port/cmake/link.cmake) and the game's calls reach the
 * t2_ names below through port/include/t2_rename.h. Threads, queues and events are t2os
 * (t2os.c); this file models the devices around them, following docs/port/RUNTIME.md:
 *
 *   VI   one retrace per T2Port_Frame (t2os_vi_retrace); mode/black/scale calls are no-ops;
 *        osViSwapBuffer records the frame buffer.
 *   SP   osSpTaskLoad/StartGo record the task; a gfx task is handed to the app
 *        (T2Port_TakeGfxTask) and its SP + DP completions are posted at the start of the next
 *        frame (rule 2: every hardware completion is posted, or the scheduler's watchdog fires).
 *   AI   audio is off for bring-up: osAiGetStatus reports the FIFO full, so __amMain never
 *        produces a frame; audio tasks, if any, complete at the next frame.
 *   PI   raw DMA copies from the ROM image (if one was given) and posts OS_EVENT_PI.
 *   SI   controllers through the app's hooks (LUS ControlDeck); every StartReadData /
 *        StartQuery posts OS_EVENT_SI (the scheduler blocks in Controller_ReadAll otherwise);
 *        no Controller Pak, no rumble pak.
 *   time a virtual clock: 46.875 MHz counted per retrace (deterministic runs).
 *   caches, TLB, interrupts, FPU control: no-ops.
 *
 * Everything here is a first model for bring-up; the ABI1 audio HLE, the virtual PIF for the
 * Controller Pak and Fast3D submission are later rounds (RUNTIME.md §5, §6, §4). */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define T2_RENAME_NO_LIBC
#include "t2_rename.h"
#include "t2os.h"
#include "t2port.h"

/* ---- logging ---- */

static T2PortConfig sCfg;
static T2PortStats sStats;

#define T2_MAX_ONCE 512
static const char *sOnce[T2_MAX_ONCE];
static int sOnceCount;

void t2_log_once(const char *what) {
    for (int i = 0; i < sOnceCount; i++)
        if (sOnce[i] == what || strcmp(sOnce[i], what) == 0)
            return;
    if (sOnceCount < T2_MAX_ONCE)
        sOnce[sOnceCount++] = what;
    fprintf(stderr, "t2: %s\n", what);
}

void t2_missing(const char *name) {
    static int abortSet = -1;
    sStats.missingCalls++;
    if (abortSet < 0)
        abortSet = getenv("T2_MISSING_ABORT") != NULL;
    for (int i = 0; i < sOnceCount; i++)
        if (sOnce[i] == name)
            goto counted;
    if (sOnceCount < T2_MAX_ONCE)
        sOnce[sOnceCount++] = name;
    fprintf(stderr, "t2: MISSING function %s called (placeholder returns 0)\n", name);
counted:
    if (abortSet) {
        fprintf(stderr, "t2: T2_MISSING_ABORT set: abort in %s\n", name);
        abort();
    }
}

/* ---- memory below 2 GB (bring-up aid) ---- */

void *t2_lowmem_alloc(size_t size) {
    size = (size + 0xFFF) & ~(size_t)0xFFF;
#if defined(__linux__) && defined(MAP_32BIT)
    if (sCfg.lowMemory) {
        void *p = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
        if (p == MAP_FAILED) {
            fprintf(stderr, "t2: mmap(MAP_32BIT, %zu) failed\n", size);
            abort();
        }
        return p;
    }
#endif
    void *p = calloc(1, size);
    if (p == NULL)
        abort();
    return p;
}

/* ---- ROM ---- */

static uint8_t *sRom;
static size_t sRomSize;

const uint8_t *t2_rom(size_t *size) {
    if (size)
        *size = sRomSize;
    return sRom;
}

static void rom_read(u32 devAddr, void *dst, u32 size) {
    /* PI addresses: the cartridge is at 0x10000000 (osRomBase); the game also passes plain
     * ROM offsets */
    u32 off = devAddr & 0x0FFFFFFF;
    sStats.piDmas++;
    sStats.piBytes += size;
    if (dst == NULL || size == 0)
        return;
    if (sRom != NULL && off < sRomSize) {
        size_t n = size;
        if (off + n > sRomSize)
            n = sRomSize - off;
        memcpy(dst, sRom + off, n);
        if (n < size)
            memset((uint8_t *)dst + n, 0, size - n);
    } else {
        memset(dst, 0, size);
    }
}

/* ---- virtual clock ---- */

#define T2_COUNT_PER_RETRACE 781250ull /* 46.875 MHz / 60 */
static u64 sTicks;    /* retraces */
static u64 sSubCount; /* advances within a retrace so successive reads differ */

u64 osGetTime(void) {
    if (sSubCount < T2_COUNT_PER_RETRACE - 64)
        sSubCount += 64;
    return sTicks * T2_COUNT_PER_RETRACE + sSubCount;
}

u32 osGetCount(void) {
    return (u32)osGetTime();
}

void osSetTime(u64 t) {
    (void)t;
    t2_log_once("osSetTime ignored (virtual clock)");
}

/* timers: osSetTimer(timer, countdown, interval, mq, msg) */
typedef struct {
    void *timer;
    u64 due, interval;
    OSMesgQueue *mq;
    OSMesg msg;
} T2Timer;
#define T2_MAX_TIMERS 32
static T2Timer sTimers[T2_MAX_TIMERS];

int osSetTimer(void *timer, u64 countdown, u64 interval, OSMesgQueue *mq, OSMesg msg) {
    T2Timer *slot = NULL;
    for (int i = 0; i < T2_MAX_TIMERS && slot == NULL; i++)
        if (sTimers[i].timer == timer)
            slot = &sTimers[i];
    for (int i = 0; i < T2_MAX_TIMERS && slot == NULL; i++)
        if (sTimers[i].timer == NULL)
            slot = &sTimers[i];
    if (slot == NULL) {
        t2_log_once("osSetTimer: too many timers");
        return -1;
    }
    slot->timer = timer;
    slot->due = osGetTime() + (countdown ? countdown : interval);
    slot->interval = interval;
    slot->mq = mq;
    slot->msg = msg;
    return 0;
}

int osStopTimer(void *timer) {
    for (int i = 0; i < T2_MAX_TIMERS; i++)
        if (sTimers[i].timer == timer) {
            sTimers[i].timer = NULL;
            return 0;
        }
    return -1;
}

static void run_timers(void) {
    u64 now = sTicks * T2_COUNT_PER_RETRACE;
    for (int i = 0; i < T2_MAX_TIMERS; i++) {
        T2Timer *t = &sTimers[i];
        if (t->timer == NULL || t->due > now)
            continue;
        t2os_post(t->mq, t->msg);
        if (t->interval)
            t->due += t->interval;
        else
            t->timer = NULL;
    }
}

/* ---- system ---- */

void osInitialize(void) {
}

u32 osVirtualToPhysical(void *p) {
    /* RUNTIME.md §5.3 wants arena offsets here (for the audio HLE); while the game still keeps
     * pointers in 32-bit words, the low 32 bits of the host pointer are what round-trips
     * (exact in lowMemory mode). */
    return (u32)(uintptr_t)p;
}

void osMapTLB(s32 index, u32 pm, void *vaddr, u32 evenpaddr, u32 oddpaddr, s32 asid) {
    (void)index, (void)pm, (void)vaddr, (void)evenpaddr, (void)oddpaddr, (void)asid;
}
void osMapTLBRdb(void) {
}
void osUnmapTLBAll(void) {
}
void __osUnmapTLBAll(void) {
}
s32 __osProbeTLB(void *vaddr) {
    return (s32)(uintptr_t)vaddr;
}
u32 osSetIntMask(u32 mask) {
    (void)mask;
    return 0;
}
void osInvalDCache(void *p, s32 n) {
    (void)p, (void)n;
}
void osInvalICache(void *p, s32 n) {
    (void)p, (void)n;
}
void osWritebackDCache(void *p, s32 n) {
    (void)p, (void)n;
}
void osWritebackDCacheAll(void) {
}
static u32 sFpcCsr = 0x01000800;
u32 __osGetFpcCsr(void) {
    return sFpcCsr;
}
u32 __osSetFpcCsr(u32 v) {
    /* mainproc enables the divide-by-zero trap (0x400): the host keeps IEEE results */
    u32 old = sFpcCsr;
    sFpcCsr = v;
    return old;
}
static u32 sSR = 0x2000FF01;
u32 __osGetSR(void) {
    return sSR;
}
void __osSetSR(u32 v) {
    sSR = v;
}
void __osSetCompare(u32 v) {
    (void)v;
}
s32 osLeoDiskInit(void) {
    return 0;
}

/* ---- VI ---- */

static void *sViCurrent, *sViNext;

void osCreateViManager(OSPri pri) {
    (void)pri; /* t2os posts VI from the platform loop (RUNTIME.md rule 4) */
}
void osViSetMode(void *mode) {
    (void)mode;
}
void osViSetSpecialFeatures(u32 f) {
    (void)f;
}
void osViSetXScale(float s) {
    (void)s;
}
void osViSetYScale(float s) {
    (void)s;
}
void osViBlack(u8 active) {
    (void)active;
}
void osViSwapBuffer(void *fb) {
    sViNext = fb;
    sViCurrent = fb;
    sStats.viSwaps++;
}
void *osViGetCurrentFramebuffer(void) {
    return sViCurrent;
}
void *osViGetNextFramebuffer(void) {
    return sViNext;
}

/* ---- SP / DP ---- */

/* The game's OSTask (include/ultra_types.h) as the host compiler lays it out. */
typedef struct {
    u32 type, flags;
    u64 *ucode_boot;
    u32 ucode_boot_size;
    u64 *ucode;
    u32 ucode_size;
    u64 *ucode_data;
    u32 ucode_data_size;
    u64 *dram_stack;
    u32 dram_stack_size;
    u64 *output_buff;
    u64 *output_buff_size;
    u64 *data_ptr;
    u32 data_size;
    u64 *yield_data_ptr;
    u32 yield_data_size;
} T2OSTask;

static const T2OSTask *sSpLoaded;
static int sSpCompletions; /* SP done events owed at the next frame */
static int sDpCompletions; /* DP done events owed at the next frame */
static T2PortTask sGfxTask;
static int sGfxTaskReady;

void osSpTaskLoad(void *task) {
    sSpLoaded = (const T2OSTask *)task;
}

void osSpTaskStartGo(void *task) {
    const T2OSTask *t = (const T2OSTask *)task;
    if (t == NULL)
        t = sSpLoaded;
    if (t == NULL) {
        t2_log_once("osSpTaskStartGo: no task");
        return;
    }
    if (sCfg.verbose)
        fprintf(stderr, "t2: task type %u flags %X data %p size %u\n", t->type, t->flags, (void *)t->data_ptr,
                t->data_size);
    switch (t->type) {
        case 1: /* M_GFXTASK: the frame's display list; RSP and RDP done at the next frame */
            sStats.gfxTasks++;
            sGfxTask.type = t->type;
            sGfxTask.flags = t->flags;
            sGfxTask.task = t;
            sGfxTask.dataPtr = t->data_ptr;
            sGfxTask.dataSize = t->data_size;
            sGfxTaskReady = 1;
            sSpCompletions++;
            sDpCompletions++;
            break;
        case 2: /* M_AUDTASK: no HLE yet (RUNTIME.md §5): complete without output */
            sStats.audioTasks++;
            t2_log_once("audio task started: no ABI1 HLE yet, completed without output");
            sSpCompletions++;
            break;
        default:
            sStats.otherTasks++;
            t2_log_once("osSpTaskStartGo: unknown task type");
            sSpCompletions++;
            break;
    }
}

s32 osSpTaskYield(void) {
    return 0;
}
u32 osSpTaskYielded(void *task) {
    (void)task;
    return 0; /* tasks complete whole; nothing is ever yielded */
}
s32 osDpSetNextBuffer(void *buf, u64 size) {
    (void)buf, (void)size;
    return 0;
}
s32 __osSpDeviceBusy(void) {
    return 0;
}
u32 __osSpGetStatus(void) {
    return 0x1; /* SP_STATUS_HALT */
}
void __osSpSetStatus(u32 v) {
    (void)v;
}
s32 __osDpDeviceBusy(void) {
    return 0;
}

int T2Port_TakeGfxTask(T2PortTask *out) {
    if (!sGfxTaskReady)
        return 0;
    sGfxTaskReady = 0;
    if (out)
        *out = sGfxTask;
    return 1;
}

/* ---- AI (off for bring-up) ---- */

static u32 sAiFrequency = 22050;

u32 osAiGetStatus(void) {
    return 0x80000000u; /* AI_STATUS_FIFO_FULL: __amMain waits for the next retrace instead */
}
u32 osAiGetLength(void) {
    return 0;
}
s32 osAiSetFrequency(u32 f) {
    sAiFrequency = f;
    return (s32)f;
}
s32 osAiSetNextBuffer(void *buf, u32 size) {
    (void)buf, (void)size;
    t2_log_once("osAiSetNextBuffer: audio output not connected yet");
    return 0;
}
s32 __osAiDeviceBusy(void) {
    return 0;
}

/* ---- PI ---- */

static u8 sPiHandle[0x100]; /* OSPiHandle (the game only passes it back to osEPi*) */
static OSMesgQueue sPiCmdQueue;

void osCreatePiManager(OSPri pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt) {
    (void)pri, (void)cmdQ, (void)cmdBuf, (void)cmdMsgCnt;
    t2_log_once("osCreatePiManager: not modelled (the loader uses raw DMA)");
}
void *osCartRomInit(void) {
    return sPiHandle;
}
OSMesgQueue *osPiGetCmdQueue(void) {
    return &sPiCmdQueue;
}
s32 osPiRawStartDma(s32 direction, u32 devAddr, void *dramAddr, u32 size) {
    if (direction != 0) {
        t2_log_once("osPiRawStartDma: write to the cartridge ignored");
        return 0;
    }
    rom_read(devAddr, dramAddr, size);
    /* the transfer is done at once; the loader waits for OS_EVENT_PI (gLoader.piReplyQueue) */
    t2os_post_event(OS_EVENT_PI);
    return 0;
}
s32 osEPiRawStartDma(void *handle, s32 direction, u32 devAddr, void *dramAddr, u32 size) {
    (void)handle;
    return osPiRawStartDma(direction, devAddr, dramAddr, size);
}
s32 osPiStartDma(void *mb, s32 priority, s32 direction, u32 devAddr, void *vAddr, u32 nbytes, OSMesgQueue *mq) {
    /* OSIoMesg request: done at once, the reply goes to mq with the request as the message */
    (void)priority;
    if (direction == 0)
        rom_read(devAddr, vAddr, nbytes);
    t2os_post(mq, mb);
    return 0;
}
s32 osEPiStartDma(void *handle, void *mb, s32 direction) {
    (void)handle, (void)mb, (void)direction;
    t2_log_once("osEPiStartDma: not modelled");
    return -1;
}
static u32 rom_word(u32 devAddr) {
    u8 b[4];
    rom_read(devAddr, b, 4);
    sStats.piDmas--;
    sStats.piBytes -= 4;
    return (u32)b[0] << 24 | (u32)b[1] << 16 | (u32)b[2] << 8 | b[3];
}
s32 osPiRawReadIo(u32 devAddr, u32 *data) {
    *data = rom_word(devAddr);
    return 0;
}
s32 osEPiRawReadIo(void *handle, u32 devAddr, u32 *data) {
    (void)handle;
    *data = rom_word(devAddr);
    return 0;
}
s32 osEPiRawWriteIo(void *handle, u32 devAddr, u32 data) {
    (void)handle, (void)devAddr, (void)data;
    return 0;
}

/* ---- SI: controllers ---- */

static T2PortContHooks sCont;
static u8 sContBits;

void T2Port_SetContHooks(const T2PortContHooks *hooks) {
    if (hooks)
        sCont = *hooks;
    else
        memset(&sCont, 0, sizeof(sCont));
}

static void si_complete(OSMesgQueue *mq) {
    /* the hardware answers through OS_EVENT_SI (gSiMessageQueue); the game's queue argument is
     * the same queue */
    sStats.siPosts++;
    t2os_post_event(OS_EVENT_SI);
    (void)mq;
}

typedef struct {
    u16 type;
    u8 status;
    u8 err;
} T2ContStatus; /* OSContStatus */

static void fill_status(T2ContStatus *st) {
    for (int i = 0; i < 4; i++) {
        if (sContBits & (1 << i)) {
            st[i].type = 0x0005; /* CONT_TYPE_NORMAL */
            st[i].status = 0;    /* no pak in the slot */
            st[i].err = 0;
        } else {
            st[i].type = 0;
            st[i].status = 0;
            st[i].err = 0x8; /* CONT_NO_RESPONSE_ERROR */
        }
    }
}

s32 osContInit(OSMesgQueue *mq, u8 *bitpattern, void *status) {
    (void)mq;
    sContBits = 0;
    if (sCont.init)
        sCont.init(&sContBits);
    if (bitpattern)
        *bitpattern = sContBits;
    if (status)
        fill_status((T2ContStatus *)status);
    return 0;
}
s32 osContReset(OSMesgQueue *mq, void *status) {
    (void)mq;
    if (status)
        fill_status((T2ContStatus *)status);
    return 0;
}
s32 osContStartQuery(OSMesgQueue *mq) {
    si_complete(mq);
    return 0;
}
void osContGetQuery(void *status) {
    if (status)
        fill_status((T2ContStatus *)status);
}
s32 osContStartReadData(OSMesgQueue *mq) {
    si_complete(mq);
    return 0;
}
void osContGetReadData(void *pads) {
    if (pads == NULL)
        return;
    memset(pads, 0, 6 * 4);
    if (sCont.read)
        sCont.read(pads);
    for (int i = 0; i < 4; i++)
        if (!(sContBits & (1 << i)))
            ((u8 *)pads)[i * 6 + 4] = 0x8; /* errno: no response */
}
s32 __osSiRawStartDma(s32 direction, void *dramAddr) {
    /* reached only from the game's Controller Pak / rumble code (__osContRamRead/Write): no pak
     * is reported, so leave the buffer and complete the transfer */
    (void)direction, (void)dramAddr;
    t2_log_once("__osSiRawStartDma: PIF not modelled (no Controller Pak yet)");
    t2os_post_event(OS_EVENT_SI);
    return 0;
}
s32 __osSiDeviceBusy(void) {
    return 0;
}
s32 osMotorInit(OSMesgQueue *mq, void *pfs, s32 channel) {
    (void)mq, (void)pfs, (void)channel;
    return 1; /* PFS_ERR_NOPACK */
}
s32 osMotorStart(void *pfs) {
    (void)pfs;
    return 1;
}
s32 osMotorStop(void *pfs) {
    (void)pfs;
    return 1;
}

/* ---- the frame (platform thread) ---- */

void t2ultra_configure(const T2PortConfig *cfg) {
    sCfg = *cfg;
}

int t2ultra_load_rom(const char *path) {
    FILE *f = fopen(path, "rb");
    if (f == NULL)
        return -1;
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    sRom = malloc((size_t)n);
    if (sRom == NULL || fread(sRom, 1, (size_t)n, f) != (size_t)n) {
        fclose(f);
        free(sRom);
        sRom = NULL;
        return -1;
    }
    fclose(f);
    sRomSize = (size_t)n;
    return 0;
}

void T2Port_Frame(void) {
    /* completions owed by tasks started during the previous frame: SP first, then DP, as
     * __scHandleRSP / __scHandleRDP expect */
    while (sSpCompletions > 0) {
        sSpCompletions--;
        t2os_post_event(OS_EVENT_SP);
    }
    while (sDpCompletions > 0) {
        sDpCompletions--;
        t2os_post_event(OS_EVENT_DP);
    }
    sTicks++;
    sSubCount = 0;
    run_timers();
    t2os_vi_retrace();
    sStats.switches += (u64)t2os_run_until_idle();
    sStats.frames++;
}

void T2Port_GetStats(T2PortStats *out) {
    *out = sStats;
}
