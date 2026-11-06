/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00256CB0.c, func_00256CB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "globals/audio.h"
OSTime osGetTime(void);
u32 osAiGetStatus(void);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern u8 gScheduler[];
extern OSMesgQueue gAudioFrameMsgQ;
extern s32 gAudioInfo[];
extern u8 gAudioGlobals[];
typedef struct B12Timer2 {
    struct B12Timer2 *next;
    u8 pad4[4];
    u16 count;
} B12Timer2;
extern B12Timer2 *gAudioDmaCache;
void osScAddClient(void *a, void *b, void *c);
void __amHandleFrameMsg(s32 buf, s32 z);
void alClose(void *g);
void __amMain(void) {
    s32 sp10[2];
    u32 msg;
    u32 cnt;
    s32 arg;
    s32 done;
    OSTime last;
    OSTime now;
    B12Timer2 *p;
    cnt = arg = done = 0;
    last = 0;
    osScAddClient(gScheduler, sp10, &gAudioFrameMsgQ);
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
                    if (p->count) {
                        p->count--;
                    }
                }
            }
            break;
        }
    } while (done == 0);
    alClose(gAudioGlobals);
}
