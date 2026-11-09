/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/IO/80200B14_2.CPP, func_80200E04_de, with Turok 2's layouts. */

#include "common.h"
#include "ultra_types.h"
#include "globals/hostio.h"
#include "protos/ultra.h"

extern void HostIo_WriteByteDma(s32 address, u8 value);
extern void HostIo_WriteWord(s32 address, s32 value);

/* Write an arbitrary byte stream to the device, using DMA when aligned
 * and falling back to byte/word writes if the DMA request is unavailable. */
void HostIo_WriteBlockDma(u32 address, u8 *source, u32 remaining) {
    s32 dmaResult = 0;

    if (sHostIoPiReady == 0) {
        dmaResult = -1;
    }
    while (remaining != 0 && (address & 3) != 0) {
        HostIo_WriteByteDma(address++, *source++);
        remaining--;
    }
    while (remaining >= 4) {
        if (remaining >= 16 && ((u32) source & 15) == 0 && dmaResult != -1) {
            dmaResult = osPiStartDma(&sHostIoDmaMsg, 1, 1, address, source, remaining, &sHostIoDmaQueue);
            if (dmaResult != -1) {
                osRecvMesg(&sHostIoDmaQueue, NULL, 1);
                remaining = 0;
            }
        } else {
            HostIo_WriteWord(address, ((u32) source[0] << 24) | ((u32) source[1] << 16) | ((u32) source[2] << 8)
                                       | source[3]);
            address += 4;
            source += 4;
            remaining -= 4;
        }
    }
    while (remaining != 0) {
        HostIo_WriteByteDma(address++, *source++);
        remaining--;
    }
}
