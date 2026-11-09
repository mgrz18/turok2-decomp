/* WRONG CUT: REAL __osPfsGetStatus size 0xD8 (splat cut it into __osPfsGetStatus + func_00428004). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SDK/PFS/80446BD0_10.CPP, func_80446D80_de, with Turok 2's layouts. */
/* __osPfsGetStatus (libultra pfsgetstatus.c). */
#include "rw/abu_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern void __osPfsRequestOneChannel(int channel);
extern void __osPfsGetOneChannelData(s32 channel, EntryRecAt3 *data);
extern s32 __osSiRawStartDma(s32 direction, void *dramAddr);

s32 __osPfsGetStatus(void *queue, int channel)
{
    s32 ret = 0;
    void *dummy;
    EntryRecAt3 data;

    __osPfsRequestOneChannel(channel);

    ret = __osSiRawStartDma(1, &__osPfsPifRam);
    osRecvMesg(queue, &dummy, 1);

    ret = __osSiRawStartDma(0, &__osPfsPifRam);
    osRecvMesg(queue, &dummy, 1);

    __osPfsGetOneChannelData(channel, &data);

    if (((data.team & 1) != 0) && ((data.team & 2) != 0)) {
        return 2;
    } else if ((data.slot != 0) || ((data.team & 1) == 0)) {
        return 1;
    } else if ((data.team & 4) != 0) {
        return 4;
    }

    return ret;
}
