/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00201ACC.c, func_00201ACC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* Matches with --cc cc1n64.exe (SN cc1n64), like the rest of this OS-side file; not with the default cc1. */

#include "common.h"
#include "ultra_types.h"
#include "globals/hostio.h"
#include "structs/hostio.h"
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
void HostIo_StartThread(s32 arg);
s32 PCinitThreaded(void) {
    HostIoRequest req;
    OSMesg reply;
    sHostIoState = 2;
    HostIo_StartThread(0xFF);
    req.size = 0;
    req.wC.flags = 0x80000101;
    req.arg0 = 0;
    req.arg1 = 0;
    req.arg2 = 0;
    req.reply = &sHostIoReplyQueue;
    osSendMesg(&sHostIoRequestQueue, &req, 1);
    osRecvMesg(&sHostIoReplyQueue, &reply, 1);
    return (s32)reply;
}
