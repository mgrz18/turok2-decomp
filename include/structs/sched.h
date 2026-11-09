#ifndef STRUCTS_SCHED_H
#define STRUCTS_SCHED_H

/* The SDK scheduler's message and client (PR/sched.h). The engine's frames
 * carry their OSScMsg in CFrameData.m_Msg (structs/frame.h). OSScTask and
 * OSSched (with Acclaim's additions, 0x80 bytes before clientList) are still
 * local views in the scheduler files and rw/abu_types.h. */

#include "common.h"
#include "ultra_types.h"

/* A message to a client: OS_SC_RETRACE_MSG (1), OS_SC_DONE_MSG (2),
 * OS_SC_PRE_NMI_MSG (4). */
typedef struct OSScMsg {
    /* 0x00 */ s16 type;
    /* 0x02 */ char misc[30];
} OSScMsg; /* size 0x20 */

/* A client: osScAddClient puts it at the head of the scheduler's list. */
typedef struct SCClient_s {
    /* 0x0 */ struct SCClient_s *next;
    /* 0x4 */ OSMesgQueue *msgQ;
} OSScClient; /* size 0x8 */

#ifdef TARGET_PORT
/* The port: the scheduler's task and the scheduler itself as typed structs (PR/sched.h's
 * OSScTask and OSSched, with Acclaim's 0x80 bytes before the client list). On the N64 the
 * scheduler files keep their local views; on the host they are replaced by port/game/t2sched.c,
 * which uses these. T2 names: rw/abu_types.h declares its own OSSched / OSScTask_s views. */
typedef struct T2ScTask {
    /* 0x00 */ struct T2ScTask *next;
    /* 0x04 */ u32 state;               /* OS_SC_NEEDS_RSP 2, OS_SC_NEEDS_RDP 1, 0x10 yielded... */
    /* 0x08 */ u32 flags;               /* OS_SC_PARALLEL_TASK 0x20, OS_SC_SWAPBUFFER 0x40... */
    /* 0x0C */ void *framebuffer;
    /* 0x10 */ OSTask list;
    /* 0x50 */ OSMesgQueue *msgQ;
    /* 0x54 */ OSMesg msg;
    /* 0x58 */ u64 startTime;
    /* 0x60 */ u64 totalTime;
} T2ScTask;

typedef struct T2Sched {
    /* 0x000 */ OSScMsg retraceMsg;     /* type 1 */
    /* 0x020 */ OSScMsg prenmiMsg;      /* type 4 */
    /* 0x040 */ OSMesgQueue interruptQ; /* Sched_GetInterruptQ */
    /* 0x058 */ OSMesg intBuf[8];
    /* 0x078 */ OSMesgQueue cmdQ;       /* osScGetCmdQ */
    /* 0x090 */ OSMesg cmdMsgBuf[8];
    /* 0x0B0 */ OSThread thread;
    /* 0x260 */ u8 unk260[0x80];
    /* 0x2E0 */ OSScClient *clientList;
    /* 0x2E4 */ T2ScTask *audioListHead;
    /* 0x2E8 */ T2ScTask *gfxListHead;
    /* 0x2EC */ T2ScTask *audioListTail;
    /* 0x2F0 */ T2ScTask *gfxListTail;
    /* 0x2F4 */ T2ScTask *curRSPTask;
    /* 0x2F8 */ T2ScTask *curRDPTask;
    /* 0x2FC */ u32 frameCount;
    /* 0x300 */ s32 doAudio;
} T2Sched;
#endif

#endif
