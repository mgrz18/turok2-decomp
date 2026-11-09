/* libultra entry points the game calls, with the SDK's own types (PR/os.h).
 *
 * Written by hand, not generated: the definitions in src/us/code are the N64's
 * libultra (or imported copies of it) and several are m2c-typed (s32 for a
 * pointer); the port replaces libultra on t2os / libultraship, so the callers
 * must see the SDK signature. Included by tools/port/genprotos.py `apply ultra`
 * wherever the N64 code stays identical. */

#ifndef PROTOS_ULTRA_H
#define PROTOS_ULTRA_H

#include "common.h"
#include "ultra_types.h"

void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
s32 osJamMesg(OSMesgQueue *, OSMesg, s32);
s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
void osSetEventMesg(u32, OSMesgQueue *, OSMesg);
void osCreateThread(OSThread *, OSId, void (*)(void *), void *, void *, OSPri);
void osStartThread(OSThread *);
void osStopThread(OSThread *);
void osDestroyThread(OSThread *);
void osSetThreadPri(OSThread *, OSPri);
OSPri osGetThreadPri(OSThread *);
void osYieldThread(void);
u32 osVirtualToPhysical(void *);
void *osPhysicalToVirtual(u32);
void osWritebackDCache(void *, s32);
void osWritebackDCacheAll(void);
void osInvalDCache(void *, s32);
void osInvalICache(void *, s32);
OSTime osGetTime(void);
u32 osGetCount(void);
s32 osSetTimer(OSTimer *, OSTime, OSTime, OSMesgQueue *, OSMesg);
s32 osStopTimer(OSTimer *);
s32 osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32, OSMesgQueue *);
s32 osAiSetNextBuffer(void *, u32);
u32 osAiGetLength(void);
s32 osAiSetFrequency(u32);
void osViSwapBuffer(void *);
void *osViGetCurrentFramebuffer(void);
void *osViGetNextFramebuffer(void);
void osViSetEvent(OSMesgQueue *, OSMesg, u32);
void osViSetMode(OSViMode *);
s32 osContStartReadData(OSMesgQueue *);
void osContGetReadData(OSContPad *);
s32 osContInit(OSMesgQueue *, u8 *, OSContStatus *);
void osSpTaskLoad(OSTask *);
void osSpTaskStartGo(OSTask *);
s32 osDpSetNextBuffer(void *, u64);

#endif
