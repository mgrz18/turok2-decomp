/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00256CB0.c, func_00256CB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/frame.h"
#include "ultra_types.h"
#include "globals/audio.h"
#include "globals/audiomgr.h"
#include "protos/ultra.h"
u32 osAiGetStatus(void);
void osScAddClient(void *a, void *b, void *c);
void __amHandleFrameMsg(AMAudioInfo *info, s32 z);
void alClose(void *g);
void __amMain(void) {
    void *sp10[2];
    u32 msg;
    u32 cnt;
    s32 arg;
    s32 done;
    OSTime last;
    OSTime now;
    AMCacheEntry *p;
    cnt = arg = done = 0;
    last = 0;
    osScAddClient(&gScheduler, sp10, &gAudioFrameMsgQ);
    do {
        now = osGetTime();
        sAmBusyTime = (u32)(((now - last) << 6) / 3000);
        osRecvMesg(&gAudioFrameMsgQ, (OSMesg *)&msg, 1);
        last = osGetTime();
        switch (msg) {
        case 4:
            break;
        case 1:
            cnt++;
            if (cnt >= 0x15) {
                cnt = 0;
        case 7:
                sAmFramesHandled++;
                while (!(osAiGetStatus() & 0x80000000)) {
                    __amHandleFrameMsg(gAudioInfo[audFrameCt % 3], arg);
                }
                for (p = gAudioDmaCache; p; p = p->next) {
                    if (p->state) {
                        p->state--;
                    }
                }
            }
            break;
        }
    } while (done == 0);
    alClose(&gAudioGlobals);
}
