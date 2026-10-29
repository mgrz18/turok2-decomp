/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* Boundary fixed in versions/cuts.us.txt: splat's func_002A2650 (0x208 bytes) is two functions: the I/O thread func_002A2650
 * (0x1B4 bytes, an endless loop with no epilogue) and func_002A2804 (0x54 bytes) right after it,
 * which starts at 0x2A2804 with its own `addiu $sp, $sp, -0x18` prologue. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002018A0.c, func_002018A0,
 * and src/game/func_00201A74.c, func_00201A74, with Turok 2's layouts. */

#include "common.h"

typedef struct OSMesgQueue OSMesgQueue;

/* Request block sent to the I/O thread through D_80128370. */
typedef struct IoRequest {
    /* 0x00 */ u8 *buffer;
    /* 0x04 */ s32 size;
    /* 0x08 */ s32 write; /* 1: send the buffer before the command, 0: read the reply into it */
    /* 0x0C */ union {
        s32 flags; /* 0x80000000: disable DMA while copying */
        struct {
            u16 hi;
            u16 cmd;
        } h;
    } wC;
    /* 0x10 */ s32 arg0;
    /* 0x14 */ s32 arg1;
    /* 0x18 */ s32 arg2;
    /* 0x1C */ OSMesgQueue *reply;
} IoRequest; /* size 0x20 */

extern s32 D_800BC300;  /* DMA allowed */
extern s32 D_800BC304;
extern OSMesgQueue D_801270C0;
extern OSMesgQueue D_80128370;
extern s32 osRecvMesg(OSMesgQueue *mq, void *msg, s32 flag);
extern s32 osSendMesg(OSMesgQueue *mq, void *msg, s32 flag);
extern u32 func_002A1384(u32 address);
extern void func_002A1350(u32 address, u32 value);
extern void func_002A1B24(u32 address, u8 *source, u32 count);
extern void func_002A1690(u32 address, u8 *dest, u32 count);
extern s32 func_002A12F0(void);
extern void func_002A1324(s32 saved);
extern s32 func_002A259C(s32 cmd, s32 arg0, s32 arg1, s32 arg2);

/* I/O thread: serve the requests sent through D_80128370 (send block, run command, read block, reply). */
void func_002A2650(void *arg) {
    IoRequest *req;
    u32 len;

    for (;;) {
        osRecvMesg(&D_80128370, &req, 1);
        if (req->size != 0 && req->write == 1) {
            if (req->wC.flags & 0x80000000) {
                D_800BC300 = 0;
            }
            func_002A1B24(0xB1FF0000, req->buffer, req->size);
            if (req->wC.flags & 0x80000000) {
                D_800BC300 = 1;
            }
        }
        osRecvMesg(&D_801270C0, NULL, 0);
        func_002A1350(0xB1FFFFE4, req->arg0);
        func_002A1350(0xB1FFFFE8, req->arg1);
        func_002A1350(0xB1FFFFEC, req->arg2);
        func_002A1350(0xB1FFFFD8, 0x1010101);
        func_002A1350(0xB1FFFFE0, req->wC.h.cmd);
        osRecvMesg(&D_801270C0, NULL, 1);
        if (req->size != 0 && req->write == 0) {
            len = func_002A1384(0xB1FFFFDC);
            if (len != 0) {
                if (req->wC.flags & 0x80000000) {
                    D_800BC300 = 0;
                }
                func_002A1690(0xB1FF0000, req->buffer, len);
                if (req->wC.flags & 0x80000000) {
                    D_800BC300 = 1;
                }
            }
        }
        if (req->reply != NULL) {
            osSendMesg(req->reply, (void *) func_002A1384(0xB1FFFFDC), 1);
        }
    }
}

