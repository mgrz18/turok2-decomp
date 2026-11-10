/* t2boot.c: the N64 boot path replaced for the native port (docs/port/LINK.md §4,
 * docs/port/RUNTIME.md §1.1 and §3).
 *
 * On the console: boot_main (SN64) -> main (func_0028D380: ROM copy, BSS clear, osInitialize)
 * -> Engine_Boot -> CEngineApp__Boot (idle thread) -> CEngineApp__Idle (creates mainproc, then
 * spins at priority 0 forever) -> mainproc -> CEngineApp__Main. On the host: T2Port_Boot() sets
 * up what the boot code would have, creates mainproc's thread directly (id 1, priority 13, on
 * the game's gThreadMain) and returns; the platform loop then calls T2Port_Frame(). The idle
 * thread is never created: under t2os a spinning thread would hold the baton forever.
 *
 * Also here: the game functions whose N64 version cannot work on the host and that are
 * replaced as a whole (their files are left out of the link, port/cmake/link.cmake):
 *   InitializeSystemResources  heap on a host allocation instead of [end of image, top of RDRAM)
 *   VM_Initialize/Alloc/Free   no pager: module 1 is linked, a VM region is its unit (t2loader.c)
 *   RNC_UnpackMethod1/2        asm on the N64 (func_00200C40/func_002010C0): port/assets/rnc.c
 *   sprintf                    SN64 varargs on the N64 (func_0029E3E0): host vsprintf
 *   func_00298470              sqrtf (sqrt.s): host libm */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define T2_RENAME_NO_LIBC
#include "t2_rename.h"
#include "t2os.h"
#include "t2port.h"
#include "rnc.h"
#include "t2audio.h"

void t2ultra_configure(const T2PortConfig *cfg);
int t2ultra_load_rom(const char *path);
void t2os_set_stack_allocator(void *(*alloc)(size_t size));

/* ---- game symbols (C in src/, or data still in asm: q2's generator / the placeholders) ---- */

extern u32 osMemSize;
extern s32 osTvType;
extern s32 osResetType;
extern s32 gWarmBoot;
extern s32 gThreadPriorityMain, gThreadPriorityScheduler, gThreadPriorityLoader, gThreadPriorityCache,
    gThreadPriorityAudio, gThreadPriorityDecompressorLow, gThreadPriorityDecompressorHigh, gThreadPriorityIdle,
    gThreadPriorityIdleInit;
extern OSThread gThreadMain;
extern u8 gHeap[], gDecompressor[], gLoader[];

void mainproc(void *arg);
void CHeap__Construct(void *heap, void *base, uintptr_t size);
void CDecompressor__Construct(void *p, s32 n);
void CLoader__Construct(void *p, s32 n);
void CCache__Construct(s32 a, s32 b);

static T2PortConfig sCfg;
static u8 *sArena;
static u32 sArenaSize;

/* ---- replaced game functions ---- */

/* func_00202130: the heap is one host allocation (the "RDRAM arena", RUNTIME.md §5.3) instead of
 * everything between the end of the image (D_8014A000) and the top of RDRAM. */
void VM_Initialize(s32 a, s32 b);
void InitializeSystemResources(void) {
    memset(sArena, 0, sArenaSize);
    CHeap__Construct(gHeap, sArena, sArenaSize);
    CDecompressor__Construct(gDecompressor, 3);
    CLoader__Construct(gLoader, 4);
    VM_Initialize(0, 0);
    CCache__Construct(0, 5);
}

/* The console's heap ends 512 KB under the top of RDRAM (InitializeSystemResources with the
 * expansion pak); code that places blocks by RDRAM address (GfxPool_Alloc: frame and depth
 * buffers under the top) asks for this instead of GetMemSize() | 0x80000000. */
uintptr_t T2Port_RdramTop(void) {
    return (uintptr_t)sArena + sArenaSize + 0x80000;
}

/* func_00275520: CAudioWorld__Initialize only with --audio (docs/port/AUDIO.md): the sound bank
 * comes converted from port/audio/t2audio_bank.c (T2Audio_LoadCtl), the sound data file is
 * swapped by VM_Alloc's hook, the audio manager is port/audio/t2audio_mgr.c. Without --audio
 * the game runs with no sound world (as before: gAudioWorldReady stays 0). */
typedef struct CAudioWorld CAudioWorld;
extern CAudioWorld gAudioWorld;
void CAudioWorld__Initialize(CAudioWorld *m);
void Sound__Initialize(void) {
    if (!T2Audio_Enabled()) {
        t2_log_once("Sound__Initialize: audio off (run with --audio)");
        return;
    }
    CAudioWorld__Initialize(&gAudioWorld);
}

/* func_0028D820: no pager, no TLB, no VM threads; module 1 is linked into the executable
 * (RUNTIME.md §1.4). */
void VM_Initialize(s32 a, s32 b) {
    (void)a, (void)b;
}

/* func_0028E60C: a VM region backed by ROM (the audio control block, the sound data file:
 * CAudioWorld__Initialize / __LoadSfxDataFile) becomes the resident unit, read through the asset
 * reader in host byte order (t2loader.c, docs/assets/PORT.md 10.4). The N64 returns the region's
 * virtual address as s32. */
void *T2Port_AssetWindow(uint32_t romAddr, uint32_t size);
intptr_t VM_Alloc(s32 size, s32 unused, u32 romAddr) {
    (void)unused;
    void *p = T2Port_AssetWindow(romAddr, (uint32_t)size);
    if (p != NULL)
        T2Audio_FixRegion(romAddr, (u8 *)p, (size_t)size); /* the sound data file: no-op when the unit is already host order */
    return (intptr_t)p;
}

/* func_0028E6DC */
void VM_Free(s32 region) {
    (void)region; /* resident copies are kept */
}

/* func_00200C40 / func_002010C0 (asm on the N64): ProPackDecompress (func_00200B00.c) calls
 *   RNC_UnpackMethod1(header + 0x12, dst, packed, unpacked)   (the data after the header)
 *   RNC_UnpackMethod2(header, dst, packed, unpacked)          (the header itself)
 * Both go to q1's clean-room decompressor (port/assets/rnc.c), which parses the 18-byte header
 * itself (big-endian, as in the ROM) and never writes past the unpacked size it states. The
 * size arguments are not used: the host would read them from big-endian ROM bytes in native
 * order, so the header is the authority. Returns the unpacked size, 0 on error. */
static s32 t2_rnc_unpack(const u8 *hdr, void *dst, const char *who) {
    RncHeader h;
    if (Rnc_ReadHeader(hdr, RNC_HEADER_LEN, &h) != RNC_OK) {
        t2_log_once("RNC_UnpackMethod: no RNC header (output left as is)");
        return 0;
    }
    int64_t n = Rnc_Unpack(hdr, RNC_HEADER_LEN + (size_t)h.packed, dst, h.unpacked, 0);
    if (n < 0) {
        fprintf(stderr, "t2: %s: %s\n", who, Rnc_ErrorString(n));
        return 0;
    }
    return (s32)n;
}
s32 RNC_UnpackMethod1(void *src, void *dst) {
    return t2_rnc_unpack((const u8 *)src - RNC_HEADER_LEN, dst, "RNC_UnpackMethod1");
}
s32 RNC_UnpackMethod2(void *src, void *dst) {
    return t2_rnc_unpack((const u8 *)src, dst, "RNC_UnpackMethod2");
}

/* func_0029E3E0: SN64's sprintf walks MIPS varargs by hand; the host has its own. (On SysV
 * x86-64 a call through the game's non-variadic local prototypes still lands here correctly;
 * on arm64 Apple variadic arguments go on the stack: those call sites need real prototypes.) */
int t2_sprintf(char *buf, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n = vsprintf(buf, fmt, ap);
    va_end(ap);
    return n;
}

/* func_00298470: libm's sqrtf (sqrt.s on the N64; the file is excluded in game.cmake) */
float func_00298470(float x) {
    return sqrtf(x);
}

/* ---- boot ---- */

static void *stack_alloc(size_t size) {
    return t2_lowmem_alloc(size);
}

/* Data the console's boot code or IPL sets, and the read-only thread priorities: written only
 * while they are still zero, so real definitions (q2's generated data) win. */
static void set_if_zero(s32 *p, s32 v) {
    if (*p == 0)
        *p = v;
}

int T2Port_Boot(const T2PortConfig *cfg) {
    sCfg = *cfg;
    if (sCfg.arenaSize == 0)
        sCfg.arenaSize = 32u << 20;
    t2ultra_configure(&sCfg);
    if (sCfg.romPath != NULL && t2ultra_load_rom(sCfg.romPath) != 0) {
        fprintf(stderr, "t2: cannot read ROM %s\n", sCfg.romPath);
        return -1;
    }
    if (sCfg.lowMemory)
        t2os_set_stack_allocator(stack_alloc);
    sArenaSize = sCfg.arenaSize;
    sArena = t2_lowmem_alloc(sArenaSize);

    /* GetMemSize() returns osMemSize: 8 MB = expansion pak (module 1 resident, hi-res modes) */
    osMemSize = 0x800000;
    set_if_zero(&osTvType, 1); /* NTSC */
    osResetType = 0;           /* cold boot */
    /* main (func_0028D380, not linked) sets gWarmBoot = 1 when the boot marker in RDRAM does not
     * match, i.e. on power-on (the name is the opposite of what it means: globals/frame.h);
     * CEngineApp__Construct then clears the saved players and runs GameSettings_SetDefaults
     * (music/sfx volumes 0xFF/0xFA, the difficulty mask 0x10, the HUD alpha, multiplayer rules).
     * Left 0, every setting stayed 0: the music played at volume 0 (docs/port/AUDIO.md) and the
     * difficulty mask CScene__DrawGeometry tests every object's flags against was 0, so no level
     * geometry was drawn (docs/port/RENDER.md). */
    gWarmBoot = 1;
    set_if_zero(&gThreadPriorityIdleInit, 10);
    set_if_zero(&gThreadPriorityDecompressorLow, 11);
    set_if_zero(&gThreadPriorityMain, 13);
    set_if_zero(&gThreadPriorityDecompressorHigh, 14);
    set_if_zero(&gThreadPriorityCache, 15);
    set_if_zero(&gThreadPriorityAudio, 16);
    set_if_zero(&gThreadPriorityLoader, 17);
    set_if_zero(&gThreadPriorityScheduler, 18);

    T2Audio_Enable(sCfg.audio);

    t2os_init();
    /* CEngineApp__Idle's work, minus the spin */
    osCreateThread(&gThreadMain, 1, sCfg.mainEntry ? sCfg.mainEntry : mainproc, NULL, NULL, gThreadPriorityMain);
    osStartThread(&gThreadMain);
    fprintf(stderr, "t2: boot: arena %u MB at %p%s, ROM %s, mainproc started (pri %d)\n", sArenaSize >> 20,
            (void *)sArena, sCfg.lowMemory ? " (low memory)" : "", sCfg.romPath ? sCfg.romPath : "none",
            (int)gThreadPriorityMain);
    return 0;
}
