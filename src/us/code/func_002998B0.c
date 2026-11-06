/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7B80_us_23.c, func_802B86F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern u32 __osPfsPifRam[];
extern u32 __osContAddressCrc(u32 arg0);
void __osPackRamReadData(s32 arg0, s32 arg1) {
    Block40 packet;
    u8 *dst;
    s32 i;
    u8 tail;
    u32 checksum;
    s32 shifted;
    dst = (u8 *)__osPfsPifRam;
    tail = 0xFF;
    __osPfsPifRam[0xF] = 1;
    packet.bytes[0] = tail;
    packet.bytes[1] = 3;
    packet.bytes[2] = 0x21;
    packet.bytes[3] = 2;
    checksum = __osContAddressCrc(arg1 & 0xFFFF);
    shifted = arg1 << 5;
    *(u16 *)&packet.bytes[4] = (checksum & 0xFF) | shifted;
    packet.bytes[0x26] = tail;
    for (i = 0; i < 32; i++) {
        ((Block40 *)((u8 *)&packet + i))->bytes[6] = 0xFF;
    }
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
