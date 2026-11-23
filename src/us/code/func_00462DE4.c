/* libultra's __osPfsSelectBank on the typed OSPfs (the m2c view below reads queue/channel as s32 at
 * N64 offsets and passes the buffer as an s32) */
#include "ultra_types.h"
extern s32 __osContRamWrite(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force);
void __osPfsSelectBank_2(OSPfs *pfs)
{
    u8 temp[32];
    int i;
    for (i = 0; i < 32; i++) {
        temp[i] = pfs->activebank;
    }
    __osContRamWrite(pfs->queue, pfs->channel, 0x8000 / 32, temp, 0);
}
