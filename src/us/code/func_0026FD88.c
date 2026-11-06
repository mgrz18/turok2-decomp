#include "common.h"

/* A frame task: a buffer at 0, its size at 4, a message block at 8, and the result range at 0x48 / 0x4C. */
typedef struct {
    /* 0x00 */ s32 buffer;
    /* 0x04 */ u16 size;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u8 msg[0x48 - 8];
    /* 0x48 */ s32 start;
    /* 0x4C */ s32 length;
} FrameTask;

s32 alAudioFrame(s32, s32, s32, s32);
s32 osAiSetNextBuffer(s32, s32);
s32 osRecvMesg(s32, s32, s32);
s32 osVirtualToPhysical(s32);
void osWritebackDCacheAll(void);

s32 __clearAudioDMA();                            /* extern */
s32 Sound__Update();                            /* extern */
s32 Sound__Lock();                            /* extern */
s32 Sound__Unlock();                            /* extern */
s32 Sched_SubmitTask(s32 *, void *);           /* extern */
extern s32 curAcmdList;
extern s32 gAudioCmdLists;
extern s32 gAudioReplyMsgQ;
extern u16 D_800F283A;
extern s32 gScheduler;

s32 __amHandleFrameMsg(FrameTask *arg0) {
    s32 sp10;
    s32 sp14;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_v0;

    __clearAudioDMA();
    temp_s0 = osVirtualToPhysical(arg0->buffer);
    arg0->size = D_800F283A;
    Sound__Lock();
    temp_s0_2 = alAudioFrame(*(((__typeof__(&gAudioCmdLists))((s8 *)&gAudioCmdLists + (curAcmdList * 4)))), (s32) &sp10, temp_s0, (s32) (s16) arg0->size);
    Sound__Unlock();
    if (sp10 == 0) {
        return 0;
    }
    temp_v0 = curAcmdList * 4;
    arg0->start = (s32) *(((__typeof__(&gAudioCmdLists))((s8 *)&gAudioCmdLists + temp_v0)));
    arg0->length = (s32) (((s32) (temp_s0_2 - *(((__typeof__(&gAudioCmdLists))((s8 *)&gAudioCmdLists + temp_v0)))) >> 3) * 8);
    osWritebackDCacheAll();
    Sched_SubmitTask(&gScheduler, arg0->msg);
    curAcmdList ^= 1;
    Sound__Update();
    osRecvMesg((s32) &gAudioReplyMsgQ, (s32) &sp14, 1);
    osAiSetNextBuffer(arg0->buffer, (s16) arg0->size * 4);
    return 1;
}
