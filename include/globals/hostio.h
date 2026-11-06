#ifndef GLOBALS_HOSTIO_H
#define GLOBALS_HOSTIO_H

/* SN Systems' host I/O (the debugger link) thread and its queues. */

#include "common.h"
#include "ultra_types.h"
#include "structs/memory.h"

extern volatile s32 sHostIoDelayCounter;
extern OSMesgQueue sHostIoEventQueue;
extern OSMesgQueue sHostIoRequestQueue;
extern OSMesgQueue sHostIoReplyQueue;
extern OSThread sHostIoThread;
extern u8 sHostIoStopped;
extern u8 sHostIoResumeAllowed;
extern u32 sHostIoSavedTlb[32][4];

/* The SN host I/O (debugger link) data, 0x800B7600 and 0x800BC300-0x800BC32F. */

/* The built-in memory map: pairs of {last address, flags}, ranges follow each other,
 * ended by last 0xFFFFFFFF; HostIo_Init patches the RAM ends. */
typedef struct MemoryBoundaryInit {
    /* 0x0 */ u32 last;
    /* 0x4 */ u32 flags;
} MemoryBoundaryInit; /* size 0x8 */
extern MemoryBoundaryInit gHostIoMemoryBounds[];

extern volatile s32 sHostIoPiReady;  /* the PI manager is started, DMA allowed */
extern s32 sHostIoState;             /* 2: the I/O thread is running */
extern u32 sHostIoTlbPageMasks[8];   /* TLB page masks 4K..16M, 0x80000000 ends it (HostIo_ScanTlb) */

#endif
