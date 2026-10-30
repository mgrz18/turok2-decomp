/* NON-MATCHING: 35 of 95 words differ: GCC reuses __osMaxControllers (D_801213DC) after the do/while instead of reloading it for the second loop, so the tail shifts by two words. */
/* WRONG CUT: the function starts at 0x00428A20 (the two words at 0x428A18 are junk before a 16-aligned
 * object) and is 0x17C bytes (splat cut it into func_00428A18 + func_00428ABC + func_00428B58).
 * REAL func_00428A20 size 0x17C at func_00428A18+0x8 */
/* osPfsIsPlug (libultra pfsisplug.c), with Turok 2's symbols. */
#include "rw/abu_types.h"

extern u8 D_801213DC; /* __osMaxControllers */
extern char D_80130E20; /* __osPfsPifRam */
extern void func_00299FE4(void); /* __osSiGetAccess */
extern void func_0029A050(void); /* __osSiRelAccess */
extern s32 func_0029A080(s32 direction, void *dramAddr); /* __osSiRawStartDma */
extern s32 osRecvMesg(void *queue, void *msg, s32 flag);
extern void func_00428B9C(u8 cmd); /* __osPfsRequestData */
extern void func_00428C38(u8 *pattern, EntryRecAt3 *data); /* __osPfsGetInitData */

s32 func_00428A20(void *queue, u8 *pattern)
{
    s32 ret = 0;
    void *dummy;
    u8 bitpattern;
    EntryRecAt3 contData[4];
    s32 channel;
    u8 bits = 0;
    s32 crcErrorCount = 3;

    func_00299FE4();

    do {
        func_00428B9C(0);

        ret = func_0029A080(1, &D_80130E20);
        osRecvMesg(queue, &dummy, 1);

        ret = func_0029A080(0, &D_80130E20);
        osRecvMesg(queue, &dummy, 1);

        func_00428C38(&bitpattern, contData);

        for (channel = 0; channel < D_801213DC; channel++) {
            if ((contData[channel].team & 4) == 0) {
                crcErrorCount--;
                break;
            }
        }

        if (channel == D_801213DC) {
            crcErrorCount = 0;
        }
    } while (crcErrorCount > 0);

    for (channel = 0; channel < D_801213DC; channel++) {
        if ((contData[channel].slot == 0) && ((contData[channel].team & 1) != 0)) {
            bits |= (1 << channel);
        }
    }

    func_0029A050();
    *pattern = bits;
    return ret;
}
