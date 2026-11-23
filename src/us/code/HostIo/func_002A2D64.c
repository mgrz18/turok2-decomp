/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00202018.c, func_00202018.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* Turok 2 builds this file with -msplit-addresses (the ROM splits %hi/%lo itself). */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */

#include "common.h"
#include "ultra_types.h"
#include "globals/hostio.h"
#include "structs/hostio.h"
#include "protos/ultra.h"
s32 HostIo_DisableInt(void);
void HostIo_RestoreInt(s32 mask);
void HostIo_WriteBlock(u32 dst, u8 *src, u32 n);
s32 HostIo_Call(u32 a0, u32 a1, u32 a2, u32 a3);
s32 PCwrite(s32 arg, u8 *buf, u32 n) {
    HostIoRequest req;
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
            req.buffer = buf;
            req.size = chunk;
            req.write = 1;
            req.wC.flags = 0x80000606;
            req.arg0 = arg;
            req.arg1 = chunk;
            req.arg2 = 0;
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
