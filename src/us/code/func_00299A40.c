/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7B80_us_25.c, func_802B8880_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern void __osSiGetAccess(void);
extern s32 __osSiRawStartDma(s32, sptr);
#ifdef TARGET_PORT
extern u8 __osContDataCrc(void *arg0); /* ABI: returns u8 (the host leaves the upper bits of the return register undefined) */
#else
extern u32 __osContDataCrc(void *arg0);
#endif
extern void __osSiRelAccess(void);
extern void __osPackRamWriteData(s32 arg0, s32 arg1, u8 *arg2);
extern s32 __osSiRawStartDma(s32, sptr);
#ifdef TARGET_PORT
extern u8 __osContDataCrc(void *arg0); /* ABI: returns u8 (the host leaves the upper bits of the return register undefined) */
#else
extern u32 __osContDataCrc(void *arg0);
#endif
#ifdef TARGET_PORT /* the queue is a pointer (s32 truncates it on LP64) */
extern u32 __osPfsGetStatus(OSMesgQueue *arg0, s32 arg1);
u32 __osContRamWrite(OSMesgQueue * arg0, s32 arg1, u16 arg2, u8 *arg3, s32 arg4) {
#else
extern u32 __osPfsGetStatus(s32 arg0, s32 arg1);
u32 __osContRamWrite(s32 arg0, s32 arg1, u16 arg2, u8 *arg3, s32 arg4) {
#endif
    Block40 saved;
    u8 *source;
    s32 i;
    s32 retries;
    u32 result;
    retries = 2;
    if (arg4 != 1 && arg2 < 7 && arg2 != 0) {
        return 0;
    }
    __osSiGetAccess();
    __osContLastCmd = 3;
    __osPackRamWriteData(arg1, arg2 & 0xFFFF, arg3);
    result = __osSiRawStartDma(1, &__osPfsPifRam);
    osRecvMesg(arg0, 0, 1);
    do {
        result = __osSiRawStartDma(0, &__osPfsPifRam);
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
            if ((__osContDataCrc(arg3) & 0xFF) != saved.bytes[0x26]) {
                result = __osPfsGetStatus(arg0, arg1);
                if (result != 0) {
                    break;
                }
                result = 4;
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
