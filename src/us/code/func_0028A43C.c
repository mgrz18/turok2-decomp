/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028ED80.c, func_0028ED80.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "globals/frame.h"
#include "structs/game.h"
#include "globals/engine.h"
#include "protos/cache.h"
#include "protos/ultra.h"
s32 osSpTaskYielded(OSTask *task);
typedef struct Qn Qn;
struct Qn { Qn *next; OSMesgQueue *q; };
typedef struct Job {
                 struct Job *next;
                 u32 flags4;
                 u32 flags8;
                 void *unkC;
                 s32 unk10;
                 u8 pad14[0x24];
                 s32 unk38;
                 u64 *unk3C;
} Job;
typedef struct Vid {
                 s16 unk0;
                 u8 pad2[0x1E];
                 s16 unk20;
                 u8 pad22[0x1E];
                 OSMesgQueue q40;
                 OSMesg buf58[8];
                 OSMesgQueue q78;
                 OSMesg buf90[8];
                 OSThread thread;
                 u8 pad260[0x80];
                 Qn *unk2E0;
                 Job *head2E4;
                 Job *head2E8;
                 Job *tail2EC;
                 Job *tail2F0;
                 Job *cur2F4;
                 Job *next2F8;
                 s32 unk2FC;
                 s32 unk300;
} Vid;
typedef struct {
    u8 pad0[0x110];
    u32 fb;
    u32 unk114;
    u8 pad118[0x28];
} View;
void __scTaskComplete(Vid *o, Job *j);
s32 __scSchedule(Vid *o, Job **a, Job **b, s32 mode);
void __scExec(Vid *o, Job *a, Job *b);
void __scHandleRSP(Vid *o) {
    Job *j;
    Job *a;
    Job *b;
    View *s;
    View *base;
    u32 fb;
    s32 mode;
    j = o->cur2F4;
    o->cur2F4 = 0;
    a = 0;
    b = 0;
    if ((j->flags4 & 0x10) && osSpTaskYielded((OSTask *)((u8 *)j + 0x10)) != 0) {
        j->flags4 |= 0x20;
        if ((j->flags8 & 7) == 3) {
            j->next = o->head2E8;
            o->head2E8 = j;
            if (o->tail2F0 == 0) {
                o->tail2F0 = j;
            }
        }
        if (j->flags8 & 0x20) {
            sGfxRspTime = sGfxRspTime + (f32)(((osGetTime() - sGfxRspResumeTime) * 64) / 3) * 0.000001f;
        }
    } else {
        j->flags4 &= ~2;
        if (j->unk10 == 1) {
            sSchedGfxWatchdog = 0;
            if (!(j->flags8 & 0x20)) {
                goto done;
            }
            sGfxRspTime = sGfxRspTime + (f32)(((osGetTime() - sGfxRspResumeTime) * 64) / 3) * 0.000001f;
            gSchedRspTotalTime = sGfxRspTime;
            gSchedRspTime = (f32)(((osGetTime() - sAudioTaskStartTime) * 64) / 3) * 0.000001f;
            base = (View *)gEngineApp.m_FrameData;
            fb = (u32)j->unkC;
            if (fb == base[0].fb) {
                s = base;
                goto sel;
            }
            if (fb == base[1].fb) {
                s = base + 1;
                goto sel;
            }
            s = base;
            if (fb == base[2].fb) {
                s += 2;
            }
sel:
            CCache__PostDisplayListDone(0, s->unk114 == gDisplayList0);
            if (osSendMesg((OSMesgQueue *)((s32 *)j)[0x50 / 4], (OSMesg)((s32 *)j)[0x54 / 4], 0 /* OS_MESG_NOBLOCK */) == -1) {
                *(u8 *)0 = 0;
            }
        } else {
            gSchedRspTime = (f32)(((osGetTime() - sAudioTaskStartTime) * 64) / 3) * 0.000001f;
            if (osSendMesg((OSMesgQueue *)((s32 *)j)[0x50 / 4], (OSMesg)((s32 *)j)[0x54 / 4], 0 /* OS_MESG_NOBLOCK */) == -1) {
                *(u8 *)0 = 0;
            }
        }
done:
        __scTaskComplete(o, j);
    }
    mode = (o->cur2F4 == 0) << 1 | (o->next2F8 == 0);
    if (__scSchedule(o, &a, &b, mode) != mode) {
        __scExec(o, a, b);
    }
}
