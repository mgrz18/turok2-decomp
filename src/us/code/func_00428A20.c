/* cc1-flags: -quiet -G0 -mips3 -mgas -meb -mno-fix4300 -mhard-float -funsigned-char -O3 -mcpu=VR4300 -mgp32 -mfp32 -fforce-addr -fno-builtin */
/* MATCHES at its real start 0x00428A20 (checked with the symbol moved there, size 0x17C) with the libultra
 * flags of its neighbour corrupted (-O3 -mfp32 -mgp32 -fforce-addr). */
/* WRONG CUT: the function starts at 0x00428A20 (the two words at 0x428A18 are junk before a 16-aligned
 * object) and is 0x17C bytes (splat cut it into func_00428A18 + func_00428ABC + func_00428B58).
 * REAL func_00428A18 size 0x17C at func_00428A18+0x8 */
/* osPfsIsPlug (libultra pfsisplug.c), with Turok 2's symbols. */
#include "rw/abu_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern void __osSiGetAccess(void); /* __osSiGetAccess */
extern void __osSiRelAccess(void); /* __osSiRelAccess */
extern s32 __osSiRawStartDma(s32 direction, void *dramAddr); /* __osSiRawStartDma */
extern void __osPfsRequestData(u8 cmd); /* __osPfsRequestData */
extern void __osPfsGetInitData(u8 *pattern, EntryRecAt3 *data); /* __osPfsGetInitData */

s32 osPfsIsPlug(void *queue, u8 *pattern)
{
    s32 ret = 0;
    void *dummy;
    u8 bitpattern;
    EntryRecAt3 contData[4];
    s32 channel;
    u8 bits = 0;
    s32 crcErrorCount = 3;

    __osSiGetAccess();

    do {
        __osPfsRequestData(0);

        ret = __osSiRawStartDma(1, &__osPfsPifRam);
        osRecvMesg(queue, &dummy, 1);

        ret = __osSiRawStartDma(0, &__osPfsPifRam);
        osRecvMesg(queue, &dummy, 1);

        __osPfsGetInitData(&bitpattern, contData);

        for (channel = 0; channel < __osContNumControllers; channel++) {
            if ((contData[channel].team & 4) == 0) {
                crcErrorCount--;
                break;
            }
        }

        if (channel == __osContNumControllers) {
            crcErrorCount = 0;
        }
    } while (crcErrorCount > 0);

    for (channel = 0; channel < __osContNumControllers; channel++) {
        if ((contData[channel].slot == 0) && ((contData[channel].team & 1) != 0)) {
            bits |= (1 << channel);
        }
    }

    __osSiRelAccess();
    *pattern = bits;
    return ret;
}
