/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/IO/80200B14_2.CPP, func_80200E04_de, with Turok 2's layouts. */

#include "common.h"

typedef struct OSMesgQueue OSMesgQueue;

extern s32 D_800BC300;          /* PI manager started */
extern u8 D_80128110[];         /* OSIoMesg used for the DMA */
extern OSMesgQueue D_801280F0;  /* its reply queue */
extern void func_002A19C8(s32 address, u8 value);
extern void func_002A1350(s32 address, s32 value);
extern s32 func_002A2FB0(void *entry, s32 pri, s32 direction, u32 devAddr, void *dramAddr, u32 size,
                         OSMesgQueue *queue);
extern s32 osRecvMesg(OSMesgQueue *mq, void *msg, s32 flag);

/* Write an arbitrary byte stream to the device, using DMA when aligned
 * and falling back to byte/word writes if the DMA request is unavailable. */
void func_002A1B24(u32 address, u8 *source, u32 remaining) {
    s32 dmaResult = 0;

    if (D_800BC300 == 0) {
        dmaResult = -1;
    }
    while (remaining != 0 && (address & 3) != 0) {
        func_002A19C8(address++, *source++);
        remaining--;
    }
    while (remaining >= 4) {
        if (remaining >= 16 && ((u32) source & 15) == 0 && dmaResult != -1) {
            dmaResult = func_002A2FB0(D_80128110, 1, 1, address, source, remaining, &D_801280F0);
            if (dmaResult != -1) {
                osRecvMesg(&D_801280F0, NULL, 1);
                remaining = 0;
            }
        } else {
            func_002A1350(address, ((u32) source[0] << 24) | ((u32) source[1] << 16) | ((u32) source[2] << 8)
                                       | source[3]);
            address += 4;
            source += 4;
            remaining -= 4;
        }
    }
    while (remaining != 0) {
        func_002A19C8(address++, *source++);
        remaining--;
    }
}
