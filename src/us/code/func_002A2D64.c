/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00202018.c, func_00202018.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* Turok 2 builds this file with -msplit-addresses (the ROM splits %hi/%lo itself). */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */

#include "common.h"
#include "ultra_types.h"
#include "globals/hostio.h"
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
typedef struct B01IoReq {
               s32 w0;
               s32 w4;
               s32 w8;
               union { s32 i; struct { u16 hi; u16 cmd; } h; } wC;
               s32 w10;
               s32 w14;
               s32 w18;
               OSMesgQueue *reply;
} B01IoReq;
extern s32 sHostIoState;
s32 HostIo_DisableInt(void);
void HostIo_RestoreInt(s32 mask);
void HostIo_WriteBlock(u32 dst, u8 *src, u32 n);
s32 HostIo_Call(u32 a0, u32 a1, u32 a2, u32 a3);
s32 PCwrite(s32 arg, u8 *buf, u32 n) {
    B01IoReq req;
    s32 reply;
    s32 done = 0;
    s32 mask;
    u32 chunk;
    if (sHostIoState == 2) {
        while (n != 0) {
            chunk = n;
            if (chunk > 0x8000) {
                chunk = 0x8000;
            }
            req.w0 = (s32)buf;
            req.w4 = chunk;
            req.w8 = 1;
            req.wC.i = 0x80000606;
            req.w10 = arg;
            req.w14 = chunk;
            req.w18 = 0;
            req.reply = &sHostIoReplyQueue;
            osSendMesg(&sHostIoRequestQueue, &req, 1);
            osRecvMesg(&sHostIoReplyQueue, (OSMesg *)&reply, 1);
            if (reply == 0) {
                return done;
            }
            buf += reply;
            n -= reply;
            done += reply;
        }
    } else {
        mask = HostIo_DisableInt();
        while (n != 0) {
            chunk = n;
            if (chunk > 0x8000) {
                chunk = 0x8000;
            }
            HostIo_WriteBlock(0xB1FF0000, buf, chunk);
            reply = HostIo_Call(0x606, arg, chunk, 0);
            if (reply == 0) {
                break;
            }
            buf += reply;
            n -= reply;
            done += reply;
        }
        HostIo_RestoreInt(mask);
    }
    return done;
}
