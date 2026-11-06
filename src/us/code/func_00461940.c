/* osPfsIsPlug (libultra 2.0I pfsisplug.c), second-module copy, with Turok 2's symbols.
 * After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SDK/PFS/80447BB0_11.CPP, func_80448470_eu_x, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x0 */ u16 type;
    /* 0x2 */ u8 status;
    /* 0x3 */ u8 errnum;
} ContStatus;

extern u8 D_801213DC;   /* __osMaxControllers */
extern char D_80130E20; /* __osPfsPifRam */
extern void __osSiGetAccess(void);                         /* __osSiGetAccess */
extern void __osSiRelAccess(void);                         /* __osSiRelAccess */
extern s32 __osSiRawStartDma(s32 direction, void *dramAddr); /* __osSiRawStartDma */
extern s32 osRecvMesg(void *queue, void *msg, s32 flag);
extern void func_00428ABC(u8 cmd);                       /* __osPfsRequestData (this module, link address) */
extern void func_00428B58(u8 *pattern, ContStatus *data); /* __osPfsGetInitData (this module, link address) */

s32 osPfsIsPlug_2(void *queue, u8 *pattern) {
    s32 ret = 0;
    void *dummy;
    u8 bitpattern;
    ContStatus data[4];
    s32 channel;
    u8 bits = 0;
    s32 crcErrorCount = 3;
    u8 *count;

    __osSiGetAccess();
    do {
        func_00428ABC(0);
        ret = __osSiRawStartDma(1, &D_80130E20);
        osRecvMesg(queue, &dummy, 1);
        ret = __osSiRawStartDma(0, &D_80130E20);
        osRecvMesg(queue, &dummy, 1);
        func_00428B58(&bitpattern, data);
        for (channel = 0; channel < D_801213DC; channel++) {
            if (!(data[channel].status & 4)) {
                crcErrorCount--;
                break;
            }
        }
        if (channel == D_801213DC) {
            crcErrorCount = 0;
        }
    } while (crcErrorCount > 0);
    count = &D_801213DC;
    for (channel = 0; channel < *count; channel++) {
        if (data[channel].errnum == 0 && (data[channel].status & 1)) {
            bits |= 1 << channel;
        }
    }
    __osSiRelAccess();
    *pattern = bits;
    return ret;
}
