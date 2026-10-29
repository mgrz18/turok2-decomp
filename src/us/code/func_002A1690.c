/* Acclaim debugger I/O: copy from cartridge space (see func_002A15B8, its no-DMA twin). */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
#include "common.h"

extern volatile s32 D_800BC300;     /* PI manager started */
extern u8 D_80128110[];             /* OSIoMesg for the DMA */
extern u8 D_801280F0[];             /* OSMesgQueue the DMA completes on */

extern u32 func_002A1384(u32 addr);           /* waits for the PI, reads a word of cartridge space */
extern s32 func_002A157C(u32 addr);           /* reads a byte of cartridge space */
extern s32 func_002A2FB0(void *mb, s32 pri, s32 direction, u32 devAddr, void *vAddr, u32 nbytes, void *mq); /* osPiStartDma */
extern s32 osRecvMesg(void *mq, void **msg, s32 flag);

/* Copies count bytes of cartridge space at source to destination: bytes up to a word boundary,
   then whole words, by DMA when the PI manager runs and the rest is 16 bytes or more to a 16-byte
   aligned destination, then the trailing bytes. */
void func_002A1690(u32 source, u8 *destination, u32 count) {
    s32 dma;

    dma = 0;
    if (D_800BC300 == 0) {
        dma = -1;
    }
    while (count != 0 && (source & 3) != 0) {
        *destination++ = func_002A157C(source++);
        count--;
    }
    while (count >= 4) {
        if (count >= 16 && !((u32)destination & 0xF) && dma != -1) {
            dma = func_002A2FB0(D_80128110, 1, 0, source, destination, count, D_801280F0);
            if (dma != -1) {
                osRecvMesg(D_801280F0, NULL, 1);
                count = 0;
            }
        } else {
            u32 word = func_002A1384(source);
            *destination++ = word >> 24;
            *destination++ = word >> 16;
            *destination++ = word >> 8;
            *destination++ = word;
            source += 4;
            count -= 4;
        }
    }
    while (count != 0) {
        *destination++ = func_002A157C(source++);
        count--;
    }
}
