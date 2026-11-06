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

#endif
