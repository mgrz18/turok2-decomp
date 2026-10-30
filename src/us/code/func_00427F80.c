/* WRONG CUT: REAL func_00427F80 size 0xD8 (splat cut it into func_00427F80 + func_00428004). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SDK/PFS/80446BD0_10.CPP, func_80446D80_de, with Turok 2's layouts. */
/* __osPfsGetStatus (libultra pfsgetstatus.c). */
#include "rw/abu_types.h"

extern void func_00428058(int channel);
extern void func_004280E4(s32 channel, EntryRecAt3 *data);
extern s32 func_0029A080(s32 direction, void *dramAddr);
extern s32 osRecvMesg(void *queue, void *msg, s32 flag);

s32 func_00427F80(void *queue, int channel)
{
    extern char D_80130E20;
    s32 ret = 0;
    void *dummy;
    EntryRecAt3 data;

    func_00428058(channel);

    ret = func_0029A080(1, &D_80130E20);
    osRecvMesg(queue, &dummy, 1);

    ret = func_0029A080(0, &D_80130E20);
    osRecvMesg(queue, &dummy, 1);

    func_004280E4(channel, &data);

    if (((data.team & 1) != 0) && ((data.team & 2) != 0)) {
        return 2;
    } else if ((data.slot != 0) || ((data.team & 1) == 0)) {
        return 1;
    } else if ((data.team & 4) != 0) {
        return 4;
    }

    return ret;
}
