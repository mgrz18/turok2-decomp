/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002016FC.c, func_002016FC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */

#include "common.h"
#include "ultra_types.h"
#include "globals/hostio.h"
#include "protos/ultra.h"
typedef u32 OSEvent;
extern u8 D_801280DC[];
void HostIo_ThreadMain(void *arg);
void HostIo_StartThread(s32 pri) {
    if (sHostIoPiReady == 0) {
        sHostIoPiReady = 1;
        osCreateMesgQueue(&sHostIoDmaQueue, sHostIoDmaBuf, 1);
        osCreateMesgQueue(&sHostIoRequestQueue, sHostIoRequestBuf, 1);
        osCreateMesgQueue(&sHostIoReplyQueue, sHostIoReplyBuf, 1);
        osCreateMesgQueue(&sHostIoEventQueue, sHostIoEventBuf, 1);
        osSetEventMesg(0, &sHostIoEventQueue, (OSMesg)1);
        osCreateThread(&sHostIoThread, 0x4653, HostIo_ThreadMain, 0, D_801280DC + 4, pri);
        osStartThread(&sHostIoThread);
    }
}
