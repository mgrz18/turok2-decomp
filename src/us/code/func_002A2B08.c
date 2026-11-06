/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00201DB0.c, func_00201DB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "globals/hostio.h"
#include "structs/hostio.h"
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
s32 HostIo_DisableInt(void);
void HostIo_RestoreInt(s32 mask);
s32 HostIo_Call(u32 a0, u32 a1, u32 a2, u32 a3);
s32 PCclose(s32 arg) {
    HostIoRequest req;
    s32 reply;
    s32 mask;
    if (sHostIoState == 2) {
        req.size = 0;
        req.wC.flags = 0x80000404;
        req.arg0 = arg;
        req.arg1 = 0;
        req.arg2 = 0;
        req.reply = &sHostIoReplyQueue;
        osSendMesg(&sHostIoRequestQueue, &req, 1);
        osRecvMesg(&sHostIoReplyQueue, (OSMesg *)&reply, 1);
        if (reply & 0x8000) {
            reply |= 0xFFFF0000;
        }
    } else {
        mask = HostIo_DisableInt();
        reply = HostIo_Call(0x404, arg, 0, 0);
        if (reply & 0x8000) {
            reply |= 0xFFFF0000;
        }
        HostIo_RestoreInt(mask);
    }
    return reply;
}
