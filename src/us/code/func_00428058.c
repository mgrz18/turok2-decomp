/* WRONG CUT: REAL __osPfsRequestOneChannel size 0x8C (splat cut it into __osPfsRequestOneChannel + func_00428080). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SDK/PFS/80446BD0_10.CPP, func_80446E58_de, with Turok 2's layouts. */
/* __osPfsRequestOneChannel (libultra pfsgetstatus.c). */
#include "rw/abu_types.h"
#include "globals/libultra.h"

void __osPfsRequestOneChannel(int channel)
{
    u8 *base;
    u8 *ptr;
    __OSContRequesFormatShort requestformat;
    int i;

    __osContLastCmd = 0;
    base = (u8 *)&__osPfsPifRam;
    __osPfsPifRam.pifstatus = 1;
    ptr = base;

    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = 0;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;

    for (i = 0; i < channel; i++) {
        *ptr++ = 0;
    }

    *(__OSContRequesFormatShort *)ptr = requestformat;
    ptr += sizeof(__OSContRequesFormatShort);
    *ptr = 0xFE;
}
