/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00202198.c, func_00202198.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "globals/hostio.h"
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
typedef struct IoReq {
               s32 unk0;
               s32 unk4;
               s32 unk8;
               u32 cmd;
               s32 a0;
               s32 a1;
               s32 a2;
               OSMesgQueue *reply;
} IoReq;
extern OSMesgQueue sHostIoRequestQueue;
extern OSMesgQueue sHostIoReplyQueue;
s32 HostIo_DisableInt();
void HostIo_RestoreInt(s32 mask);
s32 HostIo_Call(s32 a, s32 b, s32 c, s32 d);
s32 PClseek(s32 a0, s32 a1, s32 a2) {
    IoReq req;
    s32 result;
    s32 mask;
    if (sHostIoState == 2) {
        req.unk4 = 0;
        req.cmd = 0x80000707;
        req.a0 = a0;
        req.a1 = a1;
        req.a2 = a2;
        req.reply = &sHostIoReplyQueue;
        osSendMesg(&sHostIoRequestQueue, &req, 1);
        osRecvMesg(&sHostIoReplyQueue, (OSMesg *)&result, 1);
    } else {
        mask = HostIo_DisableInt();
        result = HostIo_Call(0x707, a0, a1, a2);
        HostIo_RestoreInt(mask);
    }
    return result;
}
