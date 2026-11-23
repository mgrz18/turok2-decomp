/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7B80_us_26.c, func_802B8AA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"

extern u32 __osContAddressCrc(u32 arg0);
void __osPackRamWriteData(s32 arg0, s32 arg1, u8 *arg2) {
    Block40 packet;
    u8 *dst;
    s32 i;
    u8 tail;
    u32 checksum;
    s32 shifted;
    dst = (u8 *)&__osPfsPifRam;
    tail = 0xFF;
    __osPfsPifRam.pifstatus = 1;
    packet.bytes[0] = tail;
    packet.bytes[1] = 0x23;
    packet.bytes[2] = 1;
    packet.bytes[3] = 3;
    checksum = __osContAddressCrc(arg1 & 0xFFFF);
    shifted = arg1 << 5;
#ifdef TARGET_PORT
    /* the address goes to the PIF high byte first (a u16 store is little-endian on the host) */
    packet.bytes[4] = (u8)(((checksum & 0xFF) | shifted) >> 8);
    packet.bytes[5] = (u8)((checksum & 0xFF) | shifted);
#else
    *(u16 *)&packet.bytes[4] = (checksum & 0xFF) | shifted;
#endif
    packet.bytes[0x26] = tail;
    i = 0;
    do {
        ((Block40 *)((u8 *)&packet + i))->bytes[6] = *arg2++;
        i++;
    } while (i < 0x20);
    if (arg0 != 0) {
        i = 0;
        if (arg0 > 0) {
            do {
                *dst++ = 0;
                i++;
            } while (i < arg0);
        }
    }
    *(Block40 *)dst = packet;
    dst[0x28] = 0xFE;
}
