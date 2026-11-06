#ifndef GLOBALS_LIBULTRA_H
#define GLOBALS_LIBULTRA_H

/* libultra's data (.data of the SDK objects the game links), 0x800B7A20-0x800B8990,
 * spelled and typed as decompals/ultralib has them. */

#include "common.h"
#include "ultra_types.h" /* OSThread, OSMesgQueue, OSTimer, OSViMode, __OSViContext, OSPifRam, OSPiHandle, OSIoMesg */

/* A device manager (PRinternal/osint.h's OSDevMgr): osCreatePiManager, osCreateViManager. */
typedef struct OSDevMgr {
    /* 0x00 */ s32 active;
    /* 0x04 */ OSThread *thread;
    /* 0x08 */ OSMesgQueue *cmdQueue;
    /* 0x0C */ OSMesgQueue *evtQueue;
    /* 0x10 */ OSMesgQueue *acsQueue;
    /* 0x14 */ s32 (*dma)(s32 direction, u32 devAddr, void *dramAddr, u32 size);
    /* 0x18 */ s32 (*edma)(OSPiHandle *handle, s32 direction, u32 devAddr, void *dramAddr, u32 size);
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
extern OSPiHandle *__osPiTable;
extern OSPiHandle *__osCurrentHandle[2];
extern u32 __osPiAccessQueueEnabled;

/* libultra globals, with ultralib's own types. */

/* The boot parameters the IPL3 leaves in low memory (ultralib parameters.s). */
extern s32 osTvType;            /* 0 PAL, 1 NTSC, 2 MPAL */
extern void *osRomBase;         /* cart base address for PI DMA */
extern s32 osResetType;         /* 0 cold boot, 1 NMI */
extern u32 osMemSize;
extern u8 osAppNMIBuffer[64];

/* libultra's BSS, 0x801213C0-0x80124DD0 (ultralib's names where it has them,
 * Rage Wars' otherwise, ultralib's in the comments). */

/* The controllers (controller.c, contreaddata.c, pfsisplug.c, motor.c, siacs.c). */
extern OSMesgQueue __osContMesgQueue;     /* osContInit */
extern OSMesg __osContMesgBuff;
extern u8 __osContNumControllers;         /* __osMaxControllers */
extern OSPifRam __osContBuffer;           /* __osContPifRam */
extern u8 __osContLastCmd;
extern OSPifRam __osMotorStartData[4];    /* motor.c's _MotorStartData */
extern u8 __osMotorStartBuf[32];          /* _motorstartbuf */
extern OSPifRam __osMotorStopData[4];     /* _MotorStopData */
extern u8 __osMotorStopBuf[32];           /* _motorstopbuf */
extern OSPifRam __osPfsPifRam;
extern OSMesg __osSiAccessBuf[1];         /* siAccessBuf */
extern OSMesgQueue __osSiAccessQueue;

extern OSTask __osTmpTask;                /* sptask.c's tmp_task */

/* The VI manager (vimgr.c's statics). */
extern u16 __osViRetraceCount;            /* viMgrMain's retrace */
extern OSThread __osViThread;             /* viThread */
extern u64 __osViThreadStack[0x200];      /* viThreadStack */
extern OSMesgQueue __osViEventQueue;      /* viEventQueue */
extern OSMesg __osViEventBuf[5];          /* viEventBuf */
extern OSIoMesg __osViRetraceMsg;         /* viRetraceMsg */
extern OSIoMesg __osViCounterMsg;         /* viCounterMsg */

extern OSTime osClockRate;
extern u32 __osInitialized;               /* osInitialize has run (__osFinalrom) */
extern __OSEventState __osEventStateTab[];
extern __OSEventState __osPiEventState;   /* __osEventStateTab[OS_EVENT_PI], a symbol of its own */

/* The timers (timerintr.c). */
extern u32 __osViIntrCount;
extern u32 __osBaseCounter;
extern OSTime __osCurrentTime;
extern u32 __osTimerCounter;

/* The 64DD (leodiskinit.c). */
extern OSPiHandle LeoDiskHandle;
extern OSPiHandle *__osDiskHandle;

/* The PI manager (pimgr.c's statics, piacs.c). */
extern OSThread __osPiThread;             /* piThread */
extern u64 __osPiThreadStack[0x200];      /* piThreadStack */
extern OSMesgQueue __osPiEventQueue;      /* piEventQueue */
extern OSMesg __osPiEventBuf[1];          /* piEventBuf */
extern OSMesg __osPiAccessBuf[1];         /* piAccessBuf */
extern OSMesgQueue __osPiAccessQueue;

/* The libm .rodata (cosf.c and sinf.c statics), 0x800AA750-0x800AA874. */
extern const du sCosfP[5];      /* the polynomial */
extern const du sCosfRpi;       /* 1/pi */
extern const du sCosfPiHi;      /* pi, high part */
extern const du sCosfPiLo;      /* pi, low part */
extern const fu sCosfZero;      /* 0.0f */
extern const du sSinfP[5];
extern const du sSinfRpi;
extern const du sSinfPiHi;
extern const du sSinfPiLo;
extern const fu sSinfZero;

#endif
