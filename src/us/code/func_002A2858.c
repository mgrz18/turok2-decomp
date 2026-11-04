/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00201ACC.c, func_00201ACC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* Matches with --cc cc1n64.exe (SN cc1n64), like the rest of this OS-side file; not with the default cc1. */

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
void func_002A24B4(s32 arg);
s32 func_002A2858(void) {
    B01IoReq req;
    OSMesg reply;
    D_800BC304 = 2;
    func_002A24B4(0xFF);
    req.w4 = 0;
    req.wC.i = 0x80000101;
    req.w10 = 0;
    req.w14 = 0;
    req.w18 = 0;
    req.reply = &D_80128540;
    osSendMesg(&D_80128370, &req, 1);
    osRecvMesg(&D_80128540, &reply, 1);
    return (s32)reply;
}
