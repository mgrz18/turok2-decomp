/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7B80_us.c, func_802B7B80_us.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern u32 osMotorStop(struct Triple *);
extern void __osSiGetAccess(void);
extern s32 __osSiRawStartDma(s32, sptr);
extern u32 __osContDataCrc(void *arg0);
extern void __osSiRelAccess(void);
extern s32 __osSiRawStartDma(s32, sptr);
extern u32 __osContDataCrc(void *arg0);
u32 osMotorStop(Triple *arg0) {
    Block40 saved;
    u8 *source;
    s32 i;
    source = (u8 *)&__osPfsPifRam;
    if (__osMotorinitialized[arg0->z] == 0) {
        return 5;
    } else {
        u32 result;
        s32 count;
        __osSiGetAccess();
        __osContLastCmd = 3;
        __osSiRawStartDma(1, &__osMotorStopData[arg0->z]);
        osRecvMesg(arg0->y, 0, 1);
        __osSiRawStartDma(0, &__osPfsPifRam);
        osRecvMesg(arg0->y, 0, 1);
        count = arg0->z;
        if (count != 0) {
            i = 0;
            if (count > 0) {
                do {
                    i++;
                    source++;
                } while (i < count);
            }
        }
        saved = *(Block40 *)source;
        result = (saved.bytes[2] & 0xC0) >> 4;
        if ((result == 0) && ((__osContDataCrc(&__osMotorStopBuf) & 0xFF) != saved.bytes[0x26])) {
            result = 4;
        }
        __osSiRelAccess();
        return result;
    }
}
