#ifndef PROTOS_SCHED_H
#define PROTOS_SCHED_H

/* The SDK scheduler (PR/sched.h, with Acclaim's changes) over structs/sched.h: on the N64
 * src/us/code/__scHandleRSP/__scHandleRSP.c, on the host port/game/t2sched.c. gScheduler
 * (globals/frame.h) is the engine's one OSSched. */

#include "common.h"
#include "ultra_types.h"
#include "structs/sched.h"

void __scMain(OSSched *sc);                     /* the scheduler thread */
void __scHandleRetrace(OSSched *sc);
void __scHandleRSP(OSSched *sc);
void __scHandleRDP(OSSched *sc);
s32 __scSchedule(OSSched *sc, OSScTask **sp, OSScTask **dp, s32 availRCP); /* availRCP: 2 RSP, 1 RDP */
void osCreateScheduler(OSSched *sc, void *stack, OSPri priority, u8 mode, u8 numFields);
void osScAddClient(OSSched *sc, OSScClient *c, OSMesgQueue *msgQ);
void osScRemoveClient(OSSched *sc, OSScClient *c);
OSMesgQueue *osScGetCmdQ(OSSched *sc);
void __scAppendList(OSSched *sc, OSScTask *t);
OSScTask *__scTaskReady(OSScTask *t);           /* t, unless its frame buffer is on screen or about to be */
s32 __scTaskComplete(OSSched *sc, OSScTask *t);
void __scExec(OSSched *sc, OSScTask *sp, OSScTask *dp);
void __scYield(OSSched *sc);
void Sched_HandleCmdMsg(OSSched *sc);           /* message 6: tasks waiting on cmdQ */
OSMesgQueue *Sched_GetInterruptQ(OSSched *sc);
void Sched_SubmitTask(OSSched *sc, OSScTask *t); /* onto cmdQ, then message 6 */

/* The libultra calls only the scheduler makes. */
s32 osSetIntMask(s32);
void osSpTaskYield(void);
s32 osSpTaskYielded(OSTask *task);

#endif
