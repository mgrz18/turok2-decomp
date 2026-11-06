/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/OS_2/802B7B80_28.CPP, func_802B7FD8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"
typedef struct __OSContRamReadFormat __OSContRamReadFormat;
struct __OSContRamReadFormat {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u16 address;
    u8 data[32];
    u8 datacrc;
};
struct __OSContRamReadFormat;
extern s32 __osContRamRead(OSMesgQueue *mq, int channel, u16 address, u8 *buffer);
extern s32 __osContRamWrite(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force);
extern u8 __osContAddressCrc(u16 address);
static __inline__ void _MakeMotorData(int channel, u16 address, u8 *buffer, OSPifRam *mdata) {
    u8 *ptr = (u8 *)mdata->ramarray;
    __OSContRamReadFormat ramreadformat;
    int i;
    for (i = 0; i < ((s32)(sizeof(mdata->ramarray) / sizeof(mdata->ramarray[0]))); i++) {
        mdata->ramarray[i] = 0;
    }
    mdata->pifstatus = 1;
    ramreadformat.dummy = 0xFF;
    ramreadformat.txsize = 35;
    ramreadformat.rxsize = 1;
    ramreadformat.cmd = 3;
    ramreadformat.address = (address << 0x5) | __osContAddressCrc(address);
    ramreadformat.datacrc = 0xFF;
    for (i = 0; i < ((s32)(sizeof(ramreadformat.data) / sizeof(ramreadformat.data[0]))); i++) {
        ramreadformat.data[i] = *buffer++;
    }
    if (channel != 0) {
        for (i = 0; i < channel; i++) {
            *ptr++ = 0;
        }
    }
    *(__OSContRamReadFormat *)ptr = ramreadformat;
    ptr += sizeof(__OSContRamReadFormat);
    ptr[0] = 0xFE;
}
s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel)
{
    int i;
    s32 ret;
    u8 temp[32];
    u8 *startbuf;
    u8 *stopbuf;
    pfs->queue = mq;
    pfs->channel = channel;
    pfs->status = 0;
    pfs->activebank = 128;
    for (i = 0; i < ((s32)(sizeof(temp) / sizeof(temp[0]))); i++) {
        temp[i] = 254;
    }
    ret = __osContRamWrite(mq, channel, 0x400, temp, 0);
    if (ret == 2) {
        ret = __osContRamWrite(mq, channel, 0x400, temp, 0);
    }
    if (ret != 0) {
        return ret;
    }
    ret = __osContRamRead(mq, channel, 0x400, temp);
    if (ret == 2) {
        ret = 4;
    }
    if (ret != 0) {
        return ret;
    }
    if (temp[31] == 254) {
        return 11;
    }
    for (i = 0; i < ((s32)(sizeof(temp) / sizeof(temp[0]))); i++) {
        temp[i] = 128;
    }
    ret = __osContRamWrite(mq, channel, 0x400, temp, 0);
    if (ret == 2) {
        ret = __osContRamWrite(mq, channel, 0x400, temp, 0);
    }
    if (ret != 0) {
        return ret;
    }
    ret = __osContRamRead(mq, channel, 0x400, temp);
    if (ret == 2) {
        ret = 4;
    }
    if (ret != 0) {
        return ret;
    }
    if (temp[31] != 0x80) {
        return 11;
    }
    if (!__osMotorinitialized[channel]) {
        for (i = 0; i < ((s32)(sizeof(__osMotorStartBuf) / sizeof(__osMotorStartBuf[0]))); i++) {
            startbuf = __osMotorStartBuf;
            startbuf[i] = 1;
            stopbuf = __osMotorStopBuf;
            stopbuf[i] = 0;
        }
        _MakeMotorData(channel, 0x600, __osMotorStartBuf, &__osMotorStartData[channel]);
        _MakeMotorData(channel, 0x600, __osMotorStopBuf, &__osMotorStopData[channel]);
        __osMotorinitialized[channel] = 1;
    }
    return 0;
}
