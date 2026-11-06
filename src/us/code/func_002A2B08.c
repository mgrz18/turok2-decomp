/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00201DB0.c, func_00201DB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
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
extern s32 D_800BC304;
extern OSMesgQueue D_80128370;
extern OSMesgQueue D_80128540;
s32 HostIo_DisableInt(void);
void HostIo_RestoreInt(s32 mask);
s32 HostIo_Call(u32 a0, u32 a1, u32 a2, u32 a3);
s32 PCclose(s32 arg) {
    B01IoReq req;
    s32 reply;
    s32 mask;
    if (D_800BC304 == 2) {
        req.w4 = 0;
        req.wC.i = 0x80000404;
        req.w10 = arg;
        req.w14 = 0;
        req.w18 = 0;
        req.reply = &D_80128540;
        osSendMesg(&D_80128370, &req, 1);
        osRecvMesg(&D_80128540, (OSMesg *)&reply, 1);
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
