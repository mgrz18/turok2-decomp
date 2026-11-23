#include "common.h"
#include "globals/frame.h"
#include "globals/audio.h"
#include "globals/audiomgr.h"
#include "protos/ultra.h"


sptr alAudioFrame(sptr, sptr, sptr, s32);

s32 __clearAudioDMA();                            /* extern */
s32 Sound__Update();                            /* extern */
s32 Sound__Lock();                            /* extern */
s32 Sound__Unlock();                            /* extern */
s32 Sched_SubmitTask(s32 *, void *);           /* extern */

s32 __amHandleFrameMsg(AMAudioInfo *arg0) {
    s32 sp10;
    s32 sp14;
    s32 temp_s0;
    sptr temp_s0_2;
    s32 temp_v0;

    __clearAudioDMA();
    temp_s0 = osVirtualToPhysical(arg0->data);
    arg0->frameSamples = gAudioFrameSize;
    Sound__Lock();
    temp_s0_2 = alAudioFrame((sptr) gAudioCmdLists[curAcmdList], (sptr) &sp10, temp_s0, (s32) (s16) arg0->frameSamples);
    Sound__Unlock();
    if (sp10 == 0) {
        return 0;
    }
    temp_v0 = curAcmdList * 4;
    arg0->task.list.t.data_ptr = (u64 *) *(Acmd **) ((u8 *) gAudioCmdLists + temp_v0);
    arg0->task.list.t.data_size = (((s32) (temp_s0_2 - (sptr) *(Acmd **) ((u8 *) gAudioCmdLists + temp_v0))) >> 3) * 8;
    osWritebackDCacheAll();
    Sched_SubmitTask(&gScheduler, &arg0->task);
    curAcmdList ^= 1;
    Sound__Update();
    osRecvMesg(&gAudioReplyMsgQ, &sp14, 1);
    osAiSetNextBuffer(arg0->data, (s16) arg0->frameSamples * 4);
    return 1;
}
