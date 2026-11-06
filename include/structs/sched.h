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

#endif
