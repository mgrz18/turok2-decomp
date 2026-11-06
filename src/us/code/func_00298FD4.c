/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7B80_us.c, func_802B7B80_us.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern u32 osMotorStart(struct Triple *);
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern void *D_800B7A40[];
extern u8 D_80130E20[];
extern void __osSiGetAccess(void);
extern s32 __osSiRawStartDma(s32, s32);
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern u32 __osContDataCrc(void *arg0);
extern void __osSiRelAccess(void);
extern void *D_800B7A40[];
extern u8 D_80121420;
extern u8 D_80121430[];
extern u8 D_80121530;
extern u8 D_80130E20[];
extern s32 __osSiRawStartDma(s32, s32);
extern u32 __osContDataCrc(void *arg0);
u32 osMotorStart(Triple *arg0) {
    Block40 saved;
    u8 *source;
    s32 i;
    source = D_80130E20;
    if (D_800B7A40[arg0->z] == 0) {
        return 5;
    } else {
        u32 result;
        s32 count;
        __osSiGetAccess();
        D_80121420 = 3;
        __osSiRawStartDma(1, &D_80121430[arg0->z << 6]);
        osRecvMesg(arg0->y, 0, 1);
        __osSiRawStartDma(0, D_80130E20);
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
        if ((result == 0) && ((__osContDataCrc(&D_80121530) & 0xFF) != saved.bytes[0x26])) {
            result = 4;
        }
        __osSiRelAccess();
        return result;
    }
}
