/*
 * t2os: libultra threads, message queues and events on host threads (Phase 7 prototype).
 *
 * The N64 game code assumes libultra's scheduling model: one CPU, strict priorities, a thread
 * runs until it blocks (osRecvMesg/osSendMesg on a full or empty queue), lowers its priority, or
 * wakes a thread of higher priority, which then runs at once. Interrupts (VI, SP, DP, AI, SI, PI)
 * arrive as messages. Turok 2 leans on this everywhere: CMutex and CCache use __osDisableInt as
 * their critical section, CLoader__LoadSync expects the loader thread (pri 17) to have run before
 * its osRecvMesg, the scheduler (18) preempts the engine (13) on every retrace.
 *
 * t2os keeps that model on a 64-bit host: each OSThread gets a host thread (it only holds the
 * stack), and a baton (one mutex + one condition variable per thread) lets exactly one of them run
 * at a time, chosen by libultra's rules. The platform loop (the LUS main loop) is not a game
 * thread: it posts events between frames and calls t2os_run_until_idle(), which returns when every
 * game thread is blocked. That makes each host frame a deterministic step (the same inputs give
 * the same thread interleaving), which is what Rage Wars' port calls its "oracle" mode.
 *
 * Message queues use the game's own OSMesgQueue (validCount/first/msgCount/msg), so code that
 * peeks at a queue (MQ_IS_EMPTY, osRecvMesg(..., NOBLOCK) polls) sees what it expects. OSThread
 * contents are not used: the shim keys its table by the OSThread pointer.
 *
 * Build: plain C99 + pthreads (macOS, Linux, MinGW/winpthreads). See port/runtime/Makefile.
 */
#ifndef T2OS_H
#define T2OS_H

#include <stdint.h>

#ifndef T2OS_HAVE_ULTRA_TYPES
/* Stand-alone mode (tests). The port build defines T2OS_HAVE_ULTRA_TYPES and gets the real
 * declarations from the host ultra64 headers (port/include), which must match these. */
typedef int32_t s32;
typedef uint32_t u32;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint64_t u64;
typedef void *OSMesg;
typedef s32 OSPri;
typedef s32 OSId;
typedef u32 OSEvent;
typedef struct OSThread_s {
    struct OSThread_s *next;
    OSPri priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
    OSId id;
} OSThread;
typedef struct OSMesgQueue_s {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;
#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK 1
#define OS_EVENT_SW1 0
#define OS_EVENT_SW2 1
#define OS_EVENT_CART 2
#define OS_EVENT_COUNTER 3
#define OS_EVENT_SP 4
#define OS_EVENT_SI 5
#define OS_EVENT_AI 6
#define OS_EVENT_VI 7
#define OS_EVENT_PI 8
#define OS_EVENT_DP 9
#define OS_EVENT_CPU_BREAK 10
#define OS_EVENT_SP_BREAK 11
#define OS_EVENT_FAULT 12
#define OS_EVENT_THREADSTATUS 13
#define OS_EVENT_PRENMI 14
#endif

#define T2OS_NUM_EVENTS 15
#define T2OS_MAX_THREADS 32

#ifdef __cplusplus
extern "C" {
#endif

/* libultra API (the game calls these) */
void osCreateThread(OSThread *t, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
void osStartThread(OSThread *t);
void osStopThread(OSThread *t);
void osDestroyThread(OSThread *t);
void osYieldThread(void);
void osSetThreadPri(OSThread *t, OSPri pri);
OSPri osGetThreadPri(OSThread *t);
OSId osGetThreadId(OSThread *t);
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
void osSetEventMesg(OSEvent e, OSMesgQueue *mq, OSMesg msg);
void osViSetEvent(OSMesgQueue *mq, OSMesg msg, u32 retraceCount);
/* interrupts are not used as locks on the host: one game thread runs at a time and switches only
 * inside the calls above, so a disabled-interrupt section is already atomic */
u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);

/* platform side (the LUS main loop, never a game thread) */
void t2os_init(void);
/* Raise an interrupt: the registered message goes to the registered queue (dropped if full, as
 * on hardware). Callable from the platform between runs, or from shim code on a game thread
 * (e.g. osSpTaskStartGo finishing a task at once); in the second case a woken thread of higher
 * priority runs before the call returns, as after a real interrupt. */
void t2os_post_event(OSEvent e);
/* One VI retrace: posts OS_EVENT_VI every retraceCount calls (osViSetEvent). */
void t2os_vi_retrace(void);
/* Post a message from host code without blocking (returns -1 if the queue is full). */
s32 t2os_post(OSMesgQueue *mq, OSMesg msg);
/* Run the game threads until every one of them is blocked or stopped. Returns the number of
 * thread switches made. Must be called from the platform thread. */
int t2os_run_until_idle(void);
/* Diagnostics */
typedef void (*t2os_trace_fn)(int event, OSId from, OSId to);
void t2os_set_trace(t2os_trace_fn fn);
void t2os_dump(void);
/* 1 if the caller is a game thread */
int t2os_on_game_thread(void);

#ifdef __cplusplus
}
#endif

#endif
