/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/__osRepairPackId.c, __osCheckPackId.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
s32 __osContRamRead(OSMesgQueue *mq, int channel, u16 address, u8 *buffer);
s32 __osContRamWrite(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force);
s32 __osPfsSelectBank(OSPfs *pfs);
static __inline__ s32 __osIdCheckSum(u16 *ptr, u16 *csum, u16 *icsum) {
    u16 data;
    u32 j;
    *csum = *icsum = 0;
    for (j = 0; j < 28; j += 2) {
        data = *(u16 *)((u8 *)ptr + j);
        *csum += data;
        *icsum += ~data;
    }
    return 0;
}
s32 __osCheckPackId(OSPfs *pfs, __OSPackId *temp) {
    u16 sum;
    u16 isum;
    u16 addr[4];
    s32 i;
    s32 j;
    s32 ret;
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = __osPfsSelectBank(pfs);
        if (ret != 0) {
            return ret;
        }
    }
    addr[0] = 1; addr[1] = 3; addr[2] = 4; addr[3] = 6; i = 1;
    for (; i < 4; i++) {
        ret = __osContRamRead(pfs->queue, pfs->channel, addr[i], (u8 *)temp);
        if (ret != 0) {
            return ret;
        }
        __osIdCheckSum((u16 *)temp, &sum, &isum);
        if (temp->checksum == sum && temp->inverted_checksum == isum) {
            break;
        }
    }
    if (i == 4) {
        return 10;
    }
    for (j = 0; j < 4; j++) {
        if (j != i) {
            ret = __osContRamWrite(pfs->queue, pfs->channel, addr[j], (u8 *)temp, 1);
            if (ret != 0) {
                return ret;
            }
        }
    }
    return 0;
}
