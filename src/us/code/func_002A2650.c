/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* Boundary fixed in versions/cuts.us.txt: splat's HostIo_ThreadMain (0x208 bytes) is two functions: the I/O thread HostIo_ThreadMain
 * (0x1B4 bytes, an endless loop with no epilogue) and PCinit (0x54 bytes) right after it,
 * which starts at 0x2A2804 with its own `addiu $sp, $sp, -0x18` prologue. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002018A0.c, func_002018A0,
 * and src/game/func_00201A74.c, func_00201A74, with Turok 2's layouts. */

#include "common.h"
#include "ultra_types.h"
#include "globals/hostio.h"
#include "structs/hostio.h"

extern s32 osRecvMesg(OSMesgQueue *mq, void *msg, s32 flag);
extern s32 osSendMesg(OSMesgQueue *mq, void *msg, s32 flag);
extern u32 HostIo_ReadWord(u32 address);
extern void HostIo_WriteWord(u32 address, u32 value);
extern void HostIo_WriteBlockDma(u32 address, u8 *source, u32 count);
extern void HostIo_ReadBlockDma(u32 address, u8 *dest, u32 count);
extern s32 HostIo_DisableInt(void);
extern void HostIo_RestoreInt(s32 saved);
extern s32 HostIo_Call(s32 cmd, s32 arg0, s32 arg1, s32 arg2);

/* I/O thread: serve the requests sent through sHostIoRequestQueue (send block, run command, read block, reply). */
void HostIo_ThreadMain(void *arg) {
    HostIoRequest *req;
    u32 len;

    for (;;) {
        osRecvMesg(&sHostIoRequestQueue, &req, 1);
        if (req->size != 0 && req->write == 1) {
            if (req->wC.flags & 0x80000000) {
                sHostIoPiReady = 0;
            }
            HostIo_WriteBlockDma(0xB1FF0000, req->buffer, req->size);
            if (req->wC.flags & 0x80000000) {
                sHostIoPiReady = 1;
            }
        }
        osRecvMesg(&sHostIoEventQueue, NULL, 0);
        HostIo_WriteWord(0xB1FFFFE4, req->arg0);
        HostIo_WriteWord(0xB1FFFFE8, req->arg1);
        HostIo_WriteWord(0xB1FFFFEC, req->arg2);
        HostIo_WriteWord(0xB1FFFFD8, 0x1010101);
        HostIo_WriteWord(0xB1FFFFE0, req->wC.h.cmd);
        osRecvMesg(&sHostIoEventQueue, NULL, 1);
        if (req->size != 0 && req->write == 0) {
            len = HostIo_ReadWord(0xB1FFFFDC);
            if (len != 0) {
                if (req->wC.flags & 0x80000000) {
                    sHostIoPiReady = 0;
                }
                HostIo_ReadBlockDma(0xB1FF0000, req->buffer, len);
                if (req->wC.flags & 0x80000000) {
                    sHostIoPiReady = 1;
                }
            }
        }
        if (req->reply != NULL) {
            osSendMesg(req->reply, (void *) HostIo_ReadWord(0xB1FFFFDC), 1);
        }
    }
}

