/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00201C80.c, func_00201C80, with Turok 2's layouts. */

#include "common.h"
#include "ultra_types.h"

/* Request block sent to the I/O thread (HostIo_ThreadMain) through sHostIoRequestQueue. */
typedef struct IoRequest {
    /* 0x00 */ u8 *buffer;
    /* 0x04 */ s32 size;
    /* 0x08 */ s32 write;
    /* 0x0C */ s32 flags;
    /* 0x10 */ s32 arg0;
    /* 0x14 */ s32 arg1;
    /* 0x18 */ s32 arg2;
    /* 0x1C */ OSMesgQueue *reply;
} IoRequest; /* size 0x20 */

extern s32 sHostIoState; /* 2: the I/O thread is running */
extern OSMesgQueue sHostIoRequestQueue;
extern OSMesgQueue sHostIoReplyQueue;
extern s32 osRecvMesg(OSMesgQueue *mq, void *msg, s32 flag);
extern s32 osSendMesg(OSMesgQueue *mq, void *msg, s32 flag);
extern s32 HostIo_StrSize(u8 *s);
extern s32 HostIo_DisableInt(void);
extern void HostIo_RestoreInt(s32 saved);
extern void HostIo_WriteBlock(u32 address, u8 *source, u32 length);
extern s32 HostIo_Call(s32 cmd, s32 arg0, s32 arg1, s32 arg2);

/* Send a string to the debugger host (command 0x303), through the I/O thread when it runs. */
s32 PCcreat(u8 *src, s32 arg) {
    IoRequest req;
    s32 reply;
    s32 len;
    s32 saved;

    len = HostIo_StrSize(src);
    if (sHostIoState == 2) {
        req.buffer = src;
        req.size = len;
        req.write = 1;
        req.flags = 0x80000000 | 0x303;
        req.arg0 = len;
        req.arg1 = arg;
        req.arg2 = 0;
        req.reply = &sHostIoReplyQueue;
        osSendMesg(&sHostIoRequestQueue, &req, 1);
        osRecvMesg(&sHostIoReplyQueue, &reply, 1);
        if (reply & 0x8000) {
            reply |= 0xFFFF0000;
        }
    } else {
        saved = HostIo_DisableInt();
        HostIo_WriteBlock(0xB1FF0000, src, len);
        reply = HostIo_Call(0x303, len, arg, 0);
        if (reply & 0x8000) {
            reply |= 0xFFFF0000;
        }
        HostIo_RestoreInt(saved);
    }
    return reply;
}
