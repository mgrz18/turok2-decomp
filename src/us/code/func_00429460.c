/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/__osRepairPackId.c, __osRepairPackId.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
u32 osGetCount(void);
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
s32 __osRepairPackId(OSPfs *pfs, __OSPackId *badid, __OSPackId *newid) {
    u8 buf[32];
    u8 temp2[32];
    u16 addr[4];
    s32 i;
    s32 j;
    s32 ret;
    s32 dev;
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = __osPfsSelectBank(pfs);
        if (ret != 0) {
            return ret;
        }
    }
    newid->repaired = -1;
    newid->random = osGetCount();
    newid->serial_mid = badid->serial_mid;
    newid->serial_low = badid->serial_low;
    for (i = 0; i < 62; i++) {
        pfs->activebank = i;
        ret = __osPfsSelectBank(pfs);
        if (ret != 0) {
            return ret;
        }
        ret = __osContRamRead(pfs->queue, pfs->channel, 0, buf);
        if (ret != 0) {
            return ret;
        }
        buf[0] = i | 0x80;
        for (j = 1; j < 32; j++) {
            buf[j] = ~buf[j];
        }
        ret = __osContRamWrite(pfs->queue, pfs->channel, 0, buf, 0);
        if (ret != 0) {
            return ret;
        }
        ret = __osContRamRead(pfs->queue, pfs->channel, 0, temp2);
        if (ret != 0) {
            return ret;
        }
        for (j = 0; j < 32; j++) {
            if (temp2[j] != buf[j]) {
                break;
            }
        }
        if (j != 32) {
            break;
        }
        if (i > 0) {
            pfs->activebank = 0;
            ret = __osPfsSelectBank(pfs);
            if (ret != 0) {
                return ret;
            }
            ret = __osContRamRead(pfs->queue, pfs->channel, 0, buf);
            if (ret != 0) {
                return ret;
            }
            if (buf[0] != 0x80) {
                break;
            }
        }
    }
    pfs->activebank = 0;
    ret = __osPfsSelectBank(pfs);
    if (ret != 0) {
        return ret;
    }
    dev = i > 0;
    newid->deviceid = dev | (badid->deviceid & 0xFFFE);
    newid->banks = i;
    newid->version = badid->version;
    __osIdCheckSum((u16 *)newid, &newid->checksum, &newid->inverted_checksum);
    addr[0] = 1;
    addr[1] = 3;
    addr[2] = 4;
    addr[3] = 6;
    for (j = 0; j < 4; j++) {
        ret = __osContRamWrite(pfs->queue, pfs->channel, addr[j], (u8 *)newid, 1);
        if (ret != 0) {
            return ret;
        }
    }
    ret = __osContRamRead(pfs->queue, pfs->channel, 1, buf);
    if (ret != 0) {
        return ret;
    }
    for (j = 0; j < 32; j++) {
        if (buf[j] != ((u8 *)newid)[j]) {
            return 10;
        }
    }
    return 0;
}
