#ifndef STRUCTS_HOSTIO_H
#define STRUCTS_HOSTIO_H

/* SN Systems' host I/O (the debugger link): the request a caller sends to the
 * I/O thread (HostIo_ThreadMain) through sHostIoRequestQueue; the thread
 * answers on the request's reply queue. */

#include "common.h"
#include "ultra_types.h"

typedef struct HostIoRequest {
    /* 0x00 */ u8 *buffer;
    /* 0x04 */ s32 size;
    /* 0x08 */ s32 write;           /* 1: send the buffer before the command, 0: read the reply into it */
    /* 0x0C */ union {
        s32 flags;                  /* 0x80000000: disable DMA while copying; the low half is the command */
        struct {
            u16 hi;
            u16 cmd;
        } h;
    } wC;
    /* 0x10 */ s32 arg0;
    /* 0x14 */ s32 arg1;
    /* 0x18 */ s32 arg2;
    /* 0x1C */ OSMesgQueue *reply;
} HostIoRequest; /* size 0x20 */

#endif /* STRUCTS_HOSTIO_H */
