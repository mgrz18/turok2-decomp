#ifndef GLOBALS_LIBULTRA_H
#define GLOBALS_LIBULTRA_H

/* libultra's data (.data of the SDK objects the game links), 0x800B7A20-0x800B8990,
 * spelled and typed as decompals/ultralib has them. */

#include "common.h"
#include "ultra_types.h" /* OSThread, OSMesgQueue, OSTimer, OSViMode, __OSViContext */

struct OSPiHandle_s;

/* A device manager (PRinternal/osint.h's OSDevMgr): osCreatePiManager, osCreateViManager. */
typedef struct OSDevMgr {
    /* 0x00 */ s32 active;
    /* 0x04 */ OSThread *thread;
    /* 0x08 */ OSMesgQueue *cmdQueue;
    /* 0x0C */ OSMesgQueue *evtQueue;
    /* 0x10 */ OSMesgQueue *acsQueue;
    /* 0x14 */ s32 (*dma)(s32 direction, u32 devAddr, void *dramAddr, u32 size);
    /* 0x18 */ s32 (*edma)(struct OSPiHandle_s *handle, s32 direction, u32 devAddr, void *dramAddr, u32 size);
} OSDevMgr; /* size 0x1C */

/* The end of every thread queue (__OSThreadTail). */
typedef struct __OSThreadTail {
    /* 0x0 */ OSThread *next;
    /* 0x4 */ OSPri priority; /* -1 */
} __OSThreadTail;

extern u8 hdwrBugFlag;                      /* osAiSetNextBuffer's static */
extern u32 __osContinitialized;             /* osContInit has run */
extern u32 __osMotorinitialized[4];         /* osMotorInit, per channel */
extern u32 __osSiAccessQueueEnabled;
extern __OSViContext __osViContexts[2];     /* vi.c's static vi */
extern __OSViContext *__osViCurr;
extern __OSViContext *__osViNext;
extern OSDevMgr __osViDevMgr;
extern OSViMode osViModeNtscLan1;
extern OSViMode osViModePalLan1;
extern OSViMode osViModeMpalLan1;
extern OSViMode osViModeTable[42];          /* indexed by OS_VI_* */
extern OSViMode D_800B7E60;                 /* osViModeTable[8] (NTSC HPN1) as a symbol of its own: EngineMode17_Switch_Init needs its own %hi/%lo */
extern s32 osViClock;
extern u32 __OSGlobalIntMask;
extern __OSThreadTail __osThreadTail;
extern OSThread *__osRunQueue;
extern OSThread *__osActiveQueue;
extern OSThread *__osRunningThread;
extern OSThread *__osFaultedThread;
extern OSTimer *__osTimerList;
extern OSDevMgr __osPiDevMgr;
extern struct OSPiHandle_s *__osPiTable;
extern struct OSPiHandle_s *__osCurrentHandle[2];
extern u32 __osPiAccessQueueEnabled;

#endif
