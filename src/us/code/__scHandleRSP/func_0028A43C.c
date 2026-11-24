/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028ED80.c, func_0028ED80.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"
#include "globals/frame.h"
#include "structs/game.h"
#include "globals/engine.h"
#include "protos/cache.h"

void __scHandleRSP(OSSched *o) {
    OSScTask *j;
    OSScTask *a;
    OSScTask *b;
    CFrameData *s;
    CFrameData *base;
    uptr fb;
    s32 mode;
    j = o->curRSPTask;
    o->curRSPTask = 0;
    a = 0;
    b = 0;
    if ((j->state & 0x10) && osSpTaskYielded(&j->list) != 0) {
        j->state |= 0x20;
        if ((j->flags & 7) == 3) {
            j->next = o->gfxListHead;
            o->gfxListHead = j;
            if (o->gfxListTail == 0) {
                o->gfxListTail = j;
            }
        }
        if (j->flags & 0x20) {
            sGfxRspTime = sGfxRspTime + (f32)(((osGetTime() - sGfxRspResumeTime) * 64) / 3) * 0.000001f;
        }
    } else {
        j->state &= ~2;
        if (j->list.t.type == 1) {
            sSchedGfxWatchdog = 0;
            if (!(j->flags & 0x20)) {
                goto done;
            }
            sGfxRspTime = sGfxRspTime + (f32)(((osGetTime() - sGfxRspResumeTime) * 64) / 3) * 0.000001f;
            gSchedRspTotalTime = sGfxRspTime;
            gSchedRspTime = (f32)(((osGetTime() - sAudioTaskStartTime) * 64) / 3) * 0.000001f;
            base = gEngineApp.m_FrameData;
            fb = (uptr)j->framebuffer;
            if (fb == base[0].m_pFrameBuffer) {
                s = base;
                goto sel;
            }
            if (fb == base[1].m_pFrameBuffer) {
                s = base + 1;
                goto sel;
            }
            s = base;
            if (fb == base[2].m_pFrameBuffer) {
                s += 2;
            }
sel:
            CCache__PostDisplayListDone(0, (sptr)s->m_pDisplayList == gDisplayList0);
            if (osSendMesg(j->msgQ, j->msg, 0 /* OS_MESG_NOBLOCK */) == -1) {
                *(u8 *)0 = 0;
            }
        } else {
            gSchedRspTime = (f32)(((osGetTime() - sAudioTaskStartTime) * 64) / 3) * 0.000001f;
            if (osSendMesg(j->msgQ, j->msg, 0 /* OS_MESG_NOBLOCK */) == -1) {
                *(u8 *)0 = 0;
            }
        }
done:
        __scTaskComplete(o, j);
    }
    mode = (o->curRSPTask == 0) << 1 | (o->curRDPTask == 0);
    if (__scSchedule(o, &a, &b, mode) != mode) {
        __scExec(o, a, b);
    }
}
