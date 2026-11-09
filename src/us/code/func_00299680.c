/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7B80_us_22.c, func_802B84C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern void __osSiGetAccess(void);
extern s32 __osSiRawStartDma(s32, s32);
extern u32 __osContDataCrc(void *arg0);
extern void __osSiRelAccess(void);
extern void __osPackRamReadData(s32 arg0, s32 arg1);
extern s32 __osSiRawStartDma(s32, s32);
extern u32 __osContDataCrc(void *arg0);
extern u32 __osPfsGetStatus(s32 arg0, s32 arg1);
u32 __osContRamRead(s32 arg0, s32 arg1, u16 arg2, u8 *arg3) {
    Block40 saved;
    u8 *source;
    s32 i;
    s32 retries;
    u32 result;
    source = (u8 *)&__osPfsPifRam;
    retries = 2;
    __osSiGetAccess();
    __osContLastCmd = 2;
    __osPackRamReadData(arg1, arg2 & 0xFFFF);
    __osSiRawStartDma(1, &__osPfsPifRam);
    osRecvMesg(arg0, 0, 1);
    do {
        __osSiRawStartDma(0, &__osPfsPifRam);
        osRecvMesg(arg0, 0, 1);
        source = (u8 *)&__osPfsPifRam;
        if (arg1 != 0) {
            i = 0;
            if (arg1 > 0) {
                do {
                    i++;
                    source++;
                } while (i < arg1);
            }
        }
        saved = *(Block40 *)source;
        result = (saved.bytes[2] & 0xC0) >> 4;
        if (result == 0) {
            if ((__osContDataCrc(&saved.bytes[6]) & 0xFF) != saved.bytes[0x26]) {
                result = __osPfsGetStatus(arg0, arg1);
                if (result != 0) {
                    break;
                }
                result = 4;
            } else {
                i = 0;
                do {
                    *arg3++ = ((Block40 *)((u8 *)&saved + i))->bytes[6];
                    i++;
                } while (i < 0x20);
            }
        } else {
            result = 1;
        }
        if (result != 4) {
            break;
        }
    } while (retries-- >= 0);
    __osSiRelAccess();
    return result;
}
